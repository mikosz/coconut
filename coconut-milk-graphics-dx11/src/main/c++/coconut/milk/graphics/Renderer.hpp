#ifndef _COCONUT_MILK_GRAPHICS_DX11_RENDERER_HPP_
#define _COCONUT_MILK_GRAPHICS_DX11_RENDERER_HPP_

#include <memory>

#include <d3d11.h>
#include "coconut/milk/system/cleanup-windows-macros.hpp"

#include "coconut/milk/system/COMWrapper.hpp"
#include "coconut/milk/system/Window.hpp"

#include "Texture2d.hpp"
#include "CommandList.hpp"
#include "PrimitiveTopology.hpp"

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

namespace coconut {
namespace milk {
namespace graphics {

class Device;

class Renderer {
public:

	struct Configuration {

		bool debugDevice;

		bool vsync;

		std::uint32_t sampleCount; // TODO: verify that this is a power of 2

		std::uint32_t sampleQuality;

	};

	Renderer(system::Window& window, const Configuration& configuration);

	void beginScene();

	void endScene();

	void submit(CommandList& commandList);

	Texture2d& backBuffer() {
		return backBuffer_;
	}

	ID3D11Device& internalDevice() {
		return *d3dDevice_;
	}

	ID3D11DeviceContext& internalDeviceContext() {
		return *d3dDeviceContext_;
	}

private:

	Configuration configuration_;

	system::COMWrapper<IDXGIAdapter> adapter_;

	system::COMWrapper<ID3D11Device> d3dDevice_;

	system::COMWrapper<ID3D11DeviceContext> d3dDeviceContext_;

	system::COMWrapper<IDXGISwapChain> swapChain_;

	Texture2d backBuffer_;

	Texture2d depthStencil_;

	bool vsync_;

	system::COMWrapper<ID3D11RasterizerState> rasterizer_;

};

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_DX11_RENDERER_HPP_ */
