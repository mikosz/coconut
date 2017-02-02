#ifndef _COCONUT_MILK_GRAPHICS_DX11_PIXELFORMAT_HPP_
#define _COCONUT_MILK_GRAPHICS_DX11_PIXELFORMAT_HPP_

#include <d3d11.h>
#include "coconut/milk/system/cleanup-windows-macros.hpp"

#include <coconut-tools/enum.hpp>

namespace coconut {
namespace milk {
namespace graphics {

CCN_ENUM_VALUES(
	PixelFormat,
	// colour formats
	(R32G32B32A32_FLOAT)(DXGI_FORMAT_R32G32B32A32_FLOAT)
	(R32G32B32_FLOAT)(DXGI_FORMAT_R32G32B32_FLOAT)
	(R32G32_FLOAT)(DXGI_FORMAT_R32G32_FLOAT)

	(R8G8B8A8_UNORM)(DXGI_FORMAT_R8G8B8A8_UNORM)
	(B8G8R8A8_UNORM)(DXGI_FORMAT_B8G8R8A8_UNORM)

	(R32_UINT)(DXGI_FORMAT_R32_UINT)

	// depth formats
	(D32_FLOAT)(DXGI_FORMAT_D32_FLOAT)

	// index formats
	(I16_UINT)(DXGI_FORMAT_R16_UINT)
	(I32_UINT)(DXGI_FORMAT_R32_UINT)
);

// TODO: temp
inline size_t formatSize(PixelFormat format) {
	switch (format) {
	case PixelFormat::R32G32_FLOAT:
		return 2 * 4;
	case PixelFormat::R32G32B32_FLOAT:
		return 3 * 4;
	case PixelFormat::R32G32B32A32_FLOAT:
		return 4 * 4;
	default:
		throw std::runtime_error("Unknown format size");
	}
}

inline const char* const formatHLSLType(PixelFormat format) {
	switch (format) {
	case PixelFormat::R32G32_FLOAT:
		return "float2";
	case PixelFormat::R32G32B32_FLOAT:
		return "float3";
	case PixelFormat::R32G32B32A32_FLOAT:
		return "float4";
	default:
		throw std::runtime_error("Unknown format size");
	}
}

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_DX11_PIXELFORMAT_HPP_ */
