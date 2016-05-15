#ifndef _COCONUT_MILK_GRAPHICS_DX11_PIXELFORMAT_HPP_
#define _COCONUT_MILK_GRAPHICS_DX11_PIXELFORMAT_HPP_

#include <d3d11.h>

#include <coconut-tools/enum.hpp>

namespace coconut {
namespace milk {
namespace graphics {

CCN_ENUM_VALUES(
	PixelFormat,
	// colour texture formats
	(R8G8B8A8_UNORM)(DXGI_FORMAT_R8G8B8A8_UNORM)
	(B8G8R8A8_UNORM)(DXGI_FORMAT_B8G8R8A8_UNORM)
	// depth texture formats
	(D32_FLOAT)(DXGI_FORMAT_D32_FLOAT)
	// index buffer formats
	(I16_UINT)(DXGI_FORMAT_R16_UINT)
	(I32_UINT)(DXGI_FORMAT_R32_UINT)
	);

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_DX11_PIXELFORMAT_HPP_ */
