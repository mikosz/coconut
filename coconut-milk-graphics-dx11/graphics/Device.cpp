#include "Device.hpp"

#include <vector>

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::graphics;

namespace /* anonymous */ {

void queryAdapterAndRefreshRate(
	system::Window& window,
	system::COMWrapper<IDXGIAdapter>* adapter,
	DXGI_RATIONAL* refreshRate
	) {
	HRESULT result;

	system::COMWrapper<IDXGIFactory> factory;
	if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&factory.get())))) {
		throw std::runtime_error("Failed to create a DXGIFactory");
	}

	result = factory->EnumAdapters(0, &adapter->get());
	if (result == DXGI_ERROR_NOT_FOUND) {
		throw std::runtime_error("No video cards found");
	} else if (FAILED(result)) {
		throw std::runtime_error("Failed to enumerate video cards");
	}

	system::COMWrapper<IDXGIOutput> output;
	result = (*adapter)->EnumOutputs(0, &output.get());
	if (result == DXGI_ERROR_NOT_FOUND) {
		throw std::runtime_error("No video outputs found");
	} else if (FAILED(result)) {
		throw std::runtime_error("Failed to enumerate outputs");
	}

	std::vector<DXGI_MODE_DESC> displayModes;
	UINT modeCount;
	result = output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &modeCount, 0);
	if (FAILED(result)) {
		throw std::runtime_error("Failed to get display modes");
	}

	displayModes.resize(modeCount);
	result = output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &modeCount, &displayModes.front());
	if (FAILED(result)) {
		throw std::runtime_error("Failed to get display modes");
	}

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

	swapChainDesc.BufferDesc.Width = window.clientWidth();
	swapChainDesc.BufferDesc.Height = window.clientHeight();
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

	HRESULT result = D3D11CreateDeviceAndSwapChain(
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
		);
	if (FAILED(result)) {
		throw std::runtime_error("Failed to create a directx device");
	}
}

} // anonymous namespace

Device::Device(system::Window& window, const Configuration& configuration) {
	DXGI_RATIONAL refreshRate;
	queryAdapterAndRefreshRate(window, &adapter_, &refreshRate);

	createD3DDevice(window, configuration, refreshRate, &swapChain_, &d3dDevice_, &d3dDeviceContext_);
}
