#ifndef _COCONUT_MILK_GRAPHICS_DX11_PIXELFORMAT_HPP_
#define _COCONUT_MILK_GRAPHICS_DX11_PIXELFORMAT_HPP_

#include <d3d11.h>

namespace coconut {
namespace milk {
namespace graphics {

enum PixelFormat {
	R8G8B8A8_UNORM = DXGI_FORMAT_R8G8B8A8_UNORM,
	B8G8R8A8_UNORM = DXGI_FORMAT_B8G8R8A8_UNORM,
};

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_DX11_PIXELFORMAT_HPP_ */
