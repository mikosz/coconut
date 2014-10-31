#ifndef _COCONUT_MILK_GRAPHICS_DX11_TEXTURE2D_HPP_
#define _COCONUT_MILK_GRAPHICS_DX11_TEXTURE2D_HPP_

#include <d3d11.h>

#include "PixelFormat.hpp"
#include "system/COMWrapper.hpp"
#include "utils/IntOfSize.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class Device;

class Texture2d {
public:

	enum CreationPurpose {
		SHADER_RESOURCE = D3D11_BIND_SHADER_RESOURCE,
		RENDER_TARGET = D3D11_BIND_RENDER_TARGET,
		DEPTH_STENCIL = D3D11_BIND_DEPTH_STENCIL,
	};

	enum LockPurpose {
		READ = D3D11_MAP_READ,
		WRITE = D3D11_MAP_WRITE,
		READ_WRITE = D3D11_MAP_READ_WRITE,
		WRITE_DISCARD = D3D11_MAP_WRITE_DISCARD,
		WRITE_NO_OVERWRITE = D3D11_MAP_WRITE_NO_OVERWRITE,
	};

	struct Configuration {

		size_t width;

		size_t height;

		size_t mipLevels;

		PixelFormat pixelFormat;

		bool allowModifications;

		bool allowCPURead;

		bool allowGPUWrite;

		utils::IntOfSize<sizeof(CreationPurpose)>::Unsigned purposeFlags;

	};

	Texture2d() {
	}

	Texture2d(Device& device, const Configuration& configuration, void* initialData = 0);

	Texture2d(system::COMWrapper<ID3D11Texture2D> texture) :
		texture_(texture) {
	}

	void* lock(Device& device, LockPurpose lockPurpose);

	void unlock(Device& device);

	ID3D11RenderTargetView* asRenderTargetView(Device& device);

private:

	system::COMWrapper<ID3D11Texture2D> texture_;

	system::COMWrapper<ID3D11RenderTargetView> renderTargetView_;

};

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_DX11_TEXTURE2D_HPP_ */
