#ifndef _COCONUT_MILK_GRAPHICS_DX11_PIXELFORMAT_HPP_
#define _COCONUT_MILK_GRAPHICS_DX11_PIXELFORMAT_HPP_

#include <d3d11.h>
#include "coconut/milk/system/cleanup-windows-macros.hpp"

#include <coconut-tools/enums.hpp>

namespace coconut {
namespace milk {
namespace graphics {

CT_ENUM_VALUES(
	PixelFormat,
	// colour formats
	(R32G32B32A32_FLOAT)(DXGI_FORMAT_R32G32B32A32_FLOAT)
	(R32G32B32_FLOAT)(DXGI_FORMAT_R32G32B32_FLOAT)
	(R32G32_FLOAT)(DXGI_FORMAT_R32G32_FLOAT)
	(R32_FLOAT)(DXGI_FORMAT_R32_FLOAT)

	(R8G8B8A8_UNORM_SRGB)(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB)

	//(A8R8G8B8_UNORM)(DXGI_FORMAT_A8R8G8B8_UNORM)
	(R8G8B8A8_UNORM)(DXGI_FORMAT_R8G8B8A8_UNORM)
	(B8G8R8A8_UNORM)(DXGI_FORMAT_B8G8R8A8_UNORM)
	(B8G8R8X8_UNORM)(DXGI_FORMAT_B8G8R8X8_UNORM)

	(R32_UINT)(DXGI_FORMAT_R32_UINT)

	// depth formats
	(D32_FLOAT)(DXGI_FORMAT_D32_FLOAT)

	// index formats
	(I16_UINT)(DXGI_FORMAT_R16_UINT)
	(I32_UINT)(DXGI_FORMAT_R32_UINT)
);

size_t formatSize(PixelFormat format);
const char* const formatHLSLType(PixelFormat format);

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_DX11_PIXELFORMAT_HPP_ */
