#include "Device.hpp"

#include <vector>

#include "DirectXError.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::graphics;

namespace /* anonymous */ {

void queryAdapterAndRefreshRate(
	system::Window& window,
	system::COMWrapper<IDXGIAdapter>* adapter,
	DXGI_RATIONAL* refreshRate
	) {
	system::COMWrapper<IDXGIFactory> factory;
	checkDirectXCall(
		CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&factory.get())),
		"Failed to create a DXGIFactory"
		);

	checkDirectXCall(factory->EnumAdapters(0, &adapter->get()), "Failed to enumerate video cards");

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
	const Device::Configuration& configuration,
	const DXGI_RATIONAL& refreshRate,
	system::COMWrapper<IDXGISwapChain>* swapChain,
	system::COMWrapper<ID3D11Device>* device,
	system::COMWrapper<ID3D11DeviceContext>* deviceContext
	) {
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.BufferCount = 1;

	swapChainDesc.BufferDesc.Width = static_cast<UINT>(window.clientWidth());
	swapChainDesc.BufferDesc.Height = static_cast<UINT>(window.clientHeight());
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	if (configuration.vsync) {
		swapChainDesc.BufferDesc.RefreshRate = refreshRate;
	} else {
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	swapChainDesc.OutputWindow = window.handle();

	swapChainDesc.Windowed = !window.fullscreen();

	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT creationFlags = 0;
	if (configuration.debugDevice) {
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
	}

	checkDirectXCall(
		D3D11CreateDeviceAndSwapChain(
			0,
			D3D_DRIVER_TYPE_HARDWARE,
			0,
			creationFlags,
			0,
			0,
			D3D11_SDK_VERSION,
			&swapChainDesc,
			&swapChain->get(),
			&device->get(),
			0,
			&deviceContext->get()
			),
		"Failed to create a directx device"
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
	DXGI_RATIONAL refreshRate;
	queryAdapterAndRefreshRate(window, &adapter_, &refreshRate);

	createD3DDevice(window, configuration, refreshRate, &swapChain_, &d3dDevice_, &d3dDeviceContext_);

	extractBackBuffer(swapChain_, &backBuffer_);
	setRenderTarget(backBuffer_);

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

void Device::setRenderTarget(Texture2d& texture) {
	ID3D11RenderTargetView* renderTargetView = texture.asRenderTargetView(*this);
	d3dDeviceContext_->OMSetRenderTargets(1, &renderTargetView, 0);
}

void Device::beginScene() {
	float colour[] = { 1.0f, 0.0f, 1.0f, 1.0f };
	d3dDeviceContext_->ClearRenderTargetView(backBuffer_.asRenderTargetView(*this), colour);
}

void Device::endScene() {
	swapChain_->Present(configuration_.vsync, 0);
}

void Device::draw(size_t startingIndex, size_t indexCount, PrimitiveTopology primitiveTopology) {
	d3dDeviceContext_->IASetPrimitiveTopology(static_cast<D3D11_PRIMITIVE_TOPOLOGY>(primitiveTopology));
	d3dDeviceContext_->DrawIndexed(static_cast<UINT>(indexCount), static_cast<UINT>(startingIndex), 0);
}
