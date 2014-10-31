#ifndef _COCONUT_MILK_GRAPHICS_DX11_DEVICE_HPP_
#define _COCONUT_MILK_GRAPHICS_DX11_DEVICE_HPP_

#include <d3d11.h>

#include "system/COMWrapper.hpp"
#include "system/Window.hpp"

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

	};

	Device(system::Window& window, const Configuration& configuration);

private:

	system::COMWrapper<IDXGIAdapter> adapter_;

	system::COMWrapper<ID3D11Device> d3dDevice_;

	system::COMWrapper<ID3D11DeviceContext> d3dDeviceContext_;

	system::COMWrapper<IDXGISwapChain> swapChain_;

};

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_DX11_DEVICE_HPP_ */
