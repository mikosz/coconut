#ifndef _COCONUT_MILK_GRAPHICS_DX11_TEXTURE2D_HPP_
#define _COCONUT_MILK_GRAPHICS_DX11_TEXTURE2D_HPP_

#include <d3d11.h>

#include "coconut/milk/system/COMWrapper.hpp"

#include "coconut/milk/utils/IntOfSize.hpp"
#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

#include "ShaderResource.hpp"
#include "PixelFormat.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class Device;
class Image;

class Texture2d {
public:

	enum class CreationPurpose {
		SHADER_RESOURCE = D3D11_BIND_SHADER_RESOURCE,
		RENDER_TARGET = D3D11_BIND_RENDER_TARGET,
		DEPTH_STENCIL = D3D11_BIND_DEPTH_STENCIL,
	};

	enum class LockPurpose {
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

		size_t sampleCount;

		size_t sampleQuality;

		bool allowModifications;

		bool allowCPURead;

		bool allowGPUWrite;

		std::underlying_type_t<CreationPurpose> purposeFlags;

		Configuration() {
			std::memset(this, 0, sizeof(decltype(*this))); // TODO: TEMP TEMP TEMP
			sampleCount = 1;
		}

	};

	Texture2d() {
	}

	Texture2d(Device& device, const Configuration& configuration, const void* initialData = nullptr, size_t dataRowPitch = 0); // TODO: move initialData and dataRowPitch to config

	Texture2d(Device& device, const Image& image);

	Texture2d(system::COMWrapper<ID3D11Texture2D> texture) :
		texture_(texture) {
	}

	void initialise(Device& device, const Configuration& configuration, const void* initialData = nullptr, size_t dataRowPitch = 0);

	void reset();

	void* lock(Device& device, LockPurpose lockPurpose); // TODO: return RAII object?

	void unlock(Device& device);

	ID3D11RenderTargetView* asRenderTargetView(Device& device);

	ID3D11DepthStencilView* asDepthStencilView(Device& device);

	ShaderResourceUniquePtr asShaderResource(Device& device) const;

private:

	system::COMWrapper<ID3D11Texture2D> texture_;

	system::COMWrapper<ID3D11RenderTargetView> renderTargetView_;

	system::COMWrapper<ID3D11DepthStencilView> depthStencilView_;

	mutable system::COMWrapper<ID3D11ShaderResourceView> shaderResourceView_; // TODO: mutable tempshit

};

MAKE_POINTER_DEFINITIONS(Texture2d);

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_DX11_TEXTURE2D_HPP_ */
