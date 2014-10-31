#ifndef _COCONUT_MILK_GRAPHICS_DX11_TEXTURE2D_HPP_
#define _COCONUT_MILK_GRAPHICS_DX11_TEXTURE2D_HPP_

#include <d3d11.h>

#include "Device.hpp"
#include "PixelFormat.hpp"
#include "system/COMWrapper.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class Texture2d {
public:

	enum Purpose {
		SHADER_RESOURCE = D3D11_BIND_SHADER_RESOURCE,
		RENDER_TARGET = D3D11_BIND_RENDER_TARGET,
		DEPTH_STENCIL = D3D11_BIND_DEPTH_STENCIL,
	};

	struct Configuration {

		size_t width;

		size_t height;

		size_t mipLevels;

		PixelFormat pixelFormat;

		bool allowModifications;

		bool allowCPURead;

		bool allowGPUWrite;

		purpose;

	};

	Texture2d(Device& device, const Configuration& configuration);

private:

	system::COMWrapper<ID3D11Texture2D> texture_;

};

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_DX11_TEXTURE2D_HPP_ */
