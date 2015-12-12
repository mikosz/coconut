#ifndef _COCONUT_MILK_GRAPHICS_DX11_DEVICE_HPP_
#define _COCONUT_MILK_GRAPHICS_DX11_DEVICE_HPP_

#include <d3d11.h>
#include "coconut/milk/system/cleanup-windows-macros.hpp"

#include "Texture2d.hpp"
#include "coconut/milk/system/COMWrapper.hpp"
#include "coconut/milk/system/Window.hpp"

#include "PrimitiveTopology.hpp"

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

namespace coconut {
namespace milk {
namespace graphics {

class Device {
public:

	struct Configuration {

		bool debugDevice;

		bool vsync;

		std::uint32_t sampleCount; // verify that this is a power of 2

		std::uint32_t sampleQuality;

	};

	Device(system::Window& window, const Configuration& configuration);

	void beginScene();

	void endScene();

	void draw(size_t startingIndex, size_t vertexCount, PrimitiveTopology primitiveTopology);

	void setRenderTarget(Texture2d& renderTarget, Texture2d& depthStencil);

	ID3D11Device* d3dDevice() {
		return d3dDevice_;
	}

	ID3D11DeviceContext* d3dDeviceContext() {
		return d3dDeviceContext_;
	}

	Texture2d& backBuffer() {
		return backBuffer_;
	}

private:

	Configuration configuration_;

	system::COMWrapper<IDXGIAdapter> adapter_;

	system::COMWrapper<ID3D11Device> d3dDevice_;

	system::COMWrapper<ID3D11DeviceContext> d3dDeviceContext_;

	system::COMWrapper<IDXGISwapChain> swapChain_;

	Texture2d backBuffer_;

	Texture2d depthStencil_;

	system::COMWrapper<ID3D11RasterizerState> rasterizer_;

};

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_DX11_DEVICE_HPP_ */
