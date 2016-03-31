#ifndef _COCONUT_MILK_GRAPHICS_DX11_TEXTURE2D_HPP_
#define _COCONUT_MILK_GRAPHICS_DX11_TEXTURE2D_HPP_

#include <d3d11.h>
#include "coconut/milk/system/cleanup-windows-macros.hpp"

#include "coconut/milk/system/COMWrapper.hpp"

#include "coconut/milk/utils/IntOfSize.hpp"
#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

#include "PixelFormat.hpp"
#include "Data.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class Renderer;
class Image;

class Texture2d : public Data {
public:

	enum class CreationPurpose {
		SHADER_RESOURCE = D3D11_BIND_SHADER_RESOURCE,
		RENDER_TARGET = D3D11_BIND_RENDER_TARGET,
		DEPTH_STENCIL = D3D11_BIND_DEPTH_STENCIL,
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

		const void* initialData;
		
		size_t dataRowPitch;

		Configuration() {
			std::memset(this, 0, sizeof(decltype(*this))); // TODO: TEMP TEMP TEMP
			sampleCount = 1;
		}

	};

	Texture2d() {
	}

	Texture2d(Renderer& renderer, const Configuration& configuration);

	Texture2d(Renderer& renderer, const Image& image);

	Texture2d(system::COMWrapper<ID3D11Texture2D> texture) :
		texture_(texture)
	{
	}

	void initialise(Renderer& renderer, const Configuration& configuration);

	void reset();

	ID3D11RenderTargetView& internalRenderTargetView() {
		return *renderTargetView_;
	}

	ID3D11DepthStencilView& internalDepthStencilView() {
		return *depthStencilView_;
	}

	ID3D11ShaderResourceView& internalShaderResourceView() {
		return *shaderResourceView_;
	}

	ID3D11Resource& internalResource() override {
		return *texture_;
	}

private:

	system::COMWrapper<ID3D11Texture2D> texture_;

	system::COMWrapper<ID3D11RenderTargetView> renderTargetView_;

	system::COMWrapper<ID3D11DepthStencilView> depthStencilView_;

	system::COMWrapper<ID3D11ShaderResourceView> shaderResourceView_;

};

MAKE_POINTER_DEFINITIONS(Texture2d);

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_DX11_TEXTURE2D_HPP_ */
