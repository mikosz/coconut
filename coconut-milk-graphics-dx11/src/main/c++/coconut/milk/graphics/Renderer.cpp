#include "Renderer.hpp"

#include <algorithm>

#include "DirectXError.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::graphics;

namespace /* anonymous */ {

system::COMWrapper<IDXGIFactory> createDXGIFactory() {
	system::COMWrapper<IDXGIFactory> factory;
	checkDirectXCall(
		CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&factory.get())),
		"Failed to create a DXGIFactory"
		);

	return factory;
}

void queryAdapterAndRefreshRate(
	IDXGIFactory& dxgiFactory,
	system::Window& window,
	system::COMWrapper<IDXGIAdapter>* adapter,
	DXGI_RATIONAL* refreshRate
	) {
	checkDirectXCall(dxgiFactory.EnumAdapters(0, &adapter->get()), "Failed to enumerate video cards");

	system::COMWrapper<IDXGIOutput> output;
	checkDirectXCall((*adapter)->EnumOutputs(0, &output.get()), "Failed to enumerate outputs");

	auto displayModes = std::vector<DXGI_MODE_DESC>();
	auto modeCount = UINT();
	auto displayModeListResult = HRESULT();
	displayModeListResult = output->GetDisplayModeList(
		DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &modeCount, nullptr);
	if (displayModeListResult == DXGI_ERROR_NOT_CURRENTLY_AVAILABLE) {
		modeCount = 0;
	} else {
		checkDirectXCall(displayModeListResult, "Failed to get display mode count");

		displayModes.resize(modeCount);
		checkDirectXCall(
			output->GetDisplayModeList(
				DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &modeCount, &displayModes.front()),
			"Failed to get display modes"
		);
	}

	assert(modeCount == displayModes.size());

	refreshRate->Numerator = 0;
	refreshRate->Denominator = 0;

	size_t clientWidth = window.clientWidth();
	size_t clientHeight = window.clientHeight();

	for (const auto& displayMode : displayModes) {
		if (displayMode.Width == clientWidth && displayMode.Height == clientHeight) {
			*refreshRate = displayMode.RefreshRate;
		}
	}
}

void createD3DDevice(
	system::Window& window,
	IDXGIFactory& dxgiFactory,
	const Renderer::Configuration& configuration,
	const DXGI_RATIONAL& refreshRate,
	system::COMWrapper<IDXGISwapChain>* swapChain,
	system::COMWrapper<ID3D11Device>* device,
	system::COMWrapper<ID3D11DeviceContext>* deviceContext
	) {
	UINT creationFlags = 0;
	if (configuration.debugDevice) {
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
	}

	checkDirectXCall(
		D3D11CreateDevice(
			0,
			D3D_DRIVER_TYPE_HARDWARE,
			0,
			creationFlags,
			0,
			0,
			D3D11_SDK_VERSION,
			&device->get(),
			0,
			&deviceContext->get()
			),
		"Failed to create a directx device"
		);

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.BufferCount = 1;

	swapChainDesc.BufferDesc.Width = static_cast<UINT>(window.clientWidth());
	swapChainDesc.BufferDesc.Height = static_cast<UINT>(window.clientHeight());
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // TODO: read from config or parameter
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	if (configuration.vsync) {
		swapChainDesc.BufferDesc.RefreshRate = refreshRate;
	} else {
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	UINT sampleCount = std::min<UINT>(D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT, configuration.sampleCount);
	UINT sampleQuality;

	for (;;) {
		if (sampleCount == 1) {
			sampleQuality = 0;
			break;
		}

		UINT maxSampleQuality;
		checkDirectXCall(
			(*device)->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, sampleCount, &maxSampleQuality),
			"Failed to retrieve the multisampling quality level"
			);
		if (maxSampleQuality == 0) {
			sampleCount /= 2;
		} else {
			sampleQuality = std::min<UINT>(configuration.sampleQuality, maxSampleQuality - 1);
			break;
		}
	}

	swapChainDesc.SampleDesc.Count = sampleCount;
	swapChainDesc.SampleDesc.Quality = sampleQuality;

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	swapChainDesc.OutputWindow = window.handle();

	swapChainDesc.Windowed = !window.fullscreen();

	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	checkDirectXCall(
		dxgiFactory.CreateSwapChain(
			*device,
			&swapChainDesc,
			&swapChain->get()
			),
		"Failed to create a directx swap chain"
		);
}

void extractBackBuffer(
	Renderer& renderer,
	IDXGISwapChain* swapChain,
	Texture2d* backBuffer
	)
{
	system::COMWrapper<ID3D11Texture2D> texture;
	checkDirectXCall(
		swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&texture.get())),
		"Failed to extract the back buffer texture"
		);

	backBuffer->initialise(renderer, Texture::CreationPurpose::RENDER_TARGET, texture); // TODO: this interface needs work
}

} // anonymous namespace

Renderer::Renderer(system::Window& window, const Configuration& configuration) :
	configuration_(configuration)
{
	auto dxgiFactory = createDXGIFactory();

	DXGI_RATIONAL refreshRate;
	queryAdapterAndRefreshRate(*dxgiFactory, window, &adapter_, &refreshRate);

	system::COMWrapper<ID3D11DeviceContext> immediateContext;
	createD3DDevice(window, *dxgiFactory, configuration, refreshRate, &swapChain_, &d3dDevice_, &immediateContext);
	immediateCommandList_.initialise(immediateContext);

	extractBackBuffer(*this, swapChain_, &backBuffer_);

	UINT quality;
	d3dDevice_->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &quality); // TODO: literal in code

	Texture2d::Configuration depthStencilConfig;
	depthStencilConfig.width = window.clientWidth();
	depthStencilConfig.height = window.clientHeight();
	depthStencilConfig.allowGPUWrite = true;
	depthStencilConfig.allowCPURead = false;
	depthStencilConfig.allowModifications = false;
	depthStencilConfig.mipLevels = 1;
	depthStencilConfig.arraySize = 1;
	depthStencilConfig.pixelFormat = PixelFormat::D32_FLOAT;
	depthStencilConfig.purposeFlags = Texture::CreationPurpose::DEPTH_STENCIL;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	checkDirectXCall(swapChain_->GetDesc(&swapChainDesc), "Failed to retrieve the swap chain description");
	depthStencilConfig.sampleCount = swapChainDesc.SampleDesc.Count;
	depthStencilConfig.sampleQuality = swapChainDesc.SampleDesc.Quality;

	depthStencil_.initialise(*this, depthStencilConfig);
}

CommandList& Renderer::getImmediateCommandList() {
	return immediateCommandList_;
}

CommandList Renderer::createDeferredCommandList() {
	system::COMWrapper<ID3D11DeviceContext> deferredContext;
	checkDirectXCall(
		d3dDevice_->CreateDeferredContext(0, &deferredContext.get()),
		"Failed to create a deferred context"
		);
	return CommandList(deferredContext);
}

void Renderer::beginScene() {
	// TODO: move to pulp or disperse
	float colour[] = { 1.0f, 0.0f, 1.0f, 1.0f };
	immediateCommandList_.internalDeviceContext().ClearRenderTargetView(&backBuffer_.internalRenderTargetView(), colour);
	immediateCommandList_.internalDeviceContext().ClearDepthStencilView(
		&depthStencil_.internalDepthStencilView(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		static_cast<UINT8>(0)
		);
}

void Renderer::endScene() {
	swapChain_->Present(configuration_.vsync, 0);
}

Renderer::LockedData Renderer::lock(Resource& data, LockPurpose lockPurpose) {
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	checkDirectXCall(
		immediateCommandList_.internalDeviceContext().Map(&data.internalResource(), 0, static_cast<D3D11_MAP>(lockPurpose), 0, &mappedResource),
		"Failed to map the provided resource"
		);

	return LockedData(
		mappedResource.pData,
		[&deviceContext = immediateCommandList_, &internalBuffer = data.internalResource()](void*) {
			deviceContext.internalDeviceContext().Unmap(&internalBuffer, 0);
		}
		);
}

void Renderer::submit(CommandList& commandList) {
	/*system::COMWrapper<ID3D11CommandList> d3dCommandList;
	checkDirectXCall(
		commandList.internalDeviceContext().FinishCommandList(false, &d3dCommandList.get()),
		"Failed to finish a command list"
		); // TODO: false?
	immediateCommandList_.internalDeviceContext().ExecuteCommandList(d3dCommandList, false);*/ // TODO: false?
}
