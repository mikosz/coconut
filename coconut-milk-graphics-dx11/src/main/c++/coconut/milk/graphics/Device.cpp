#include "Device.hpp"

#include <vector>
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

	std::vector<DXGI_MODE_DESC> displayModes;
	UINT modeCount;
	checkDirectXCall(
		output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &modeCount, 0),
		"Failed to get display mode count"
		);

	displayModes.resize(modeCount);
	checkDirectXCall(
		output->GetDisplayModeList(
			DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &modeCount, &displayModes.front()),
		"Failed to get display modes"
		);

	assert(modeCount == displayModes.size());

	refreshRate->Numerator = 0;
	refreshRate->Denominator = 0;

	size_t clientWidth = window.clientWidth();
	size_t clientHeight = window.clientHeight();

	for (size_t i = 0; i< displayModes.size(); ++i) {
		if (displayModes[i].Width == clientWidth && displayModes[i].Height == clientHeight) {
			*refreshRate = displayModes[i].RefreshRate;
		}
	}
}

void createD3DDevice(
	system::Window& window,
	IDXGIFactory& dxgiFactory,
	const Device::Configuration& configuration,
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
	IDXGISwapChain* swapChain,
	Texture2d* backBuffer
	) {
	system::COMWrapper<ID3D11Texture2D> texture;
	checkDirectXCall(
		swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&texture.get())),
		"Failed to extract the back buffer texture"
		);

	*backBuffer = texture;
}

} // anonymous namespace

Device::Device(system::Window& window, const Configuration& configuration) :
	configuration_(configuration)
{
	auto dxgiFactory = createDXGIFactory();

	DXGI_RATIONAL refreshRate;
	queryAdapterAndRefreshRate(*dxgiFactory, window, &adapter_, &refreshRate);

	createD3DDevice(window, *dxgiFactory, configuration, refreshRate, &swapChain_, &d3dDevice_, &d3dDeviceContext_);

	extractBackBuffer(swapChain_, &backBuffer_);

	UINT quality;
	d3dDevice_->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &quality);

	Texture2d::Configuration depthStencilConfig;
	depthStencilConfig.width = window.clientWidth();
	depthStencilConfig.height = window.clientHeight();
	depthStencilConfig.allowGPUWrite = true;
	depthStencilConfig.allowCPURead = false;
	depthStencilConfig.allowModifications = false;
	depthStencilConfig.mipLevels = 1;
	depthStencilConfig.pixelFormat = PixelFormat::D32_FLOAT;
	depthStencilConfig.purposeFlags = static_cast<std::underlying_type_t<Texture2d::CreationPurpose>>(Texture2d::CreationPurpose::DEPTH_STENCIL);

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	checkDirectXCall(swapChain_->GetDesc(&swapChainDesc), "Failed to retrieve the swap chain description");
	depthStencilConfig.sampleCount = swapChainDesc.SampleDesc.Count;
	depthStencilConfig.sampleQuality = swapChainDesc.SampleDesc.Quality;
	
	depthStencil_.initialise(*this, depthStencilConfig); // TODO: cleanup initialisation + do we need a default depth stencil view?

	setRenderTarget(backBuffer_, depthStencil_);

	D3D11_VIEWPORT viewport;
	viewport.Height = static_cast<float>(window.clientHeight());
	viewport.Width = static_cast<float>(window.clientWidth());
	viewport.MaxDepth = D3D11_MAX_DEPTH;
	viewport.MinDepth = D3D11_MIN_DEPTH;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	d3dDeviceContext_->RSSetViewports(1, &viewport);

	D3D11_RASTERIZER_DESC rasterizer;
	std::memset(&rasterizer, 0, sizeof(rasterizer));
	rasterizer.CullMode = D3D11_CULL_NONE;
	rasterizer.FillMode = D3D11_FILL_SOLID;
	d3dDevice_->CreateRasterizerState(&rasterizer, &rasterizer_.get());

	d3dDeviceContext_->RSSetState(rasterizer_);
}

void Device::setRenderTarget(Texture2d& renderTarget, Texture2d& depthStencil) {
	auto* renderTargetView = renderTarget.asRenderTargetView(*this);
	auto* depthStencilView = depthStencil.asDepthStencilView(*this);
	d3dDeviceContext_->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
}

void Device::beginScene() {
	float colour[] = { 1.0f, 0.0f, 1.0f, 1.0f };
	d3dDeviceContext_->ClearRenderTargetView(backBuffer_.asRenderTargetView(*this), colour);
	d3dDeviceContext_->ClearDepthStencilView(
		depthStencil_.asDepthStencilView(*this),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		static_cast<UINT8>(0)
		);
}

void Device::endScene() {
	swapChain_->Present(configuration_.vsync, 0);
}

void Device::draw(size_t startingIndex, size_t indexCount, PrimitiveTopology primitiveTopology) {
	d3dDeviceContext_->IASetPrimitiveTopology(static_cast<D3D11_PRIMITIVE_TOPOLOGY>(primitiveTopology));
	d3dDeviceContext_->DrawIndexed(static_cast<UINT>(indexCount), static_cast<UINT>(startingIndex), 0);
}
