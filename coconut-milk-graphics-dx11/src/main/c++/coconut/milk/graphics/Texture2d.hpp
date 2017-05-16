#ifndef _COCONUT_MILK_GRAPHICS_DX11_TEXTURE2D_HPP_
#define _COCONUT_MILK_GRAPHICS_DX11_TEXTURE2D_HPP_

#include <coconut/milk/system/COMWrapper.hpp>
#include "PixelFormat.hpp"
#include "Texture.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class Renderer;
class Image;

class Texture2d : public Texture {
public:

	struct Configuration {

		size_t width = 0u;

		size_t height = 0u;

		size_t arraySize = 1u;

		size_t mipLevels = 1u;

		PixelFormat pixelFormat;

		size_t sampleCount = 1u;

		size_t sampleQuality = 0u;

		bool allowModifications;

		bool allowCPURead;

		bool allowGPUWrite;

		coconut_tools::Mask<CreationPurpose> purpose;

		const void* initialData = nullptr;
		
		size_t dataRowPitch = 0u;

	};

	Texture2d() = default;

	Texture2d(system::COMWrapper<ID3D11Texture2D> internalTexture) :
		Texture(internalTexture)
	{
	}

	Texture2d(Renderer& renderer, const Configuration& configuration);

	Texture2d(Renderer& renderer, const Image& image);

};

class RenderTargetView {
public:

	RenderTargetView() = default;

	RenderTargetView(Renderer& renderer, const Texture2d& texture);

	ID3D11RenderTargetView* internal() const noexcept {
		return rtv_.get();
	}

private:

	system::COMWrapper<ID3D11RenderTargetView> rtv_;

};

class DepthStencilView {
public:

	DepthStencilView() = default;

	DepthStencilView(Renderer& renderer, const Texture2d& texture);

	ID3D11DepthStencilView* internal() const noexcept {
		return dsv_.get();
	}

private:

	system::COMWrapper<ID3D11DepthStencilView> dsv_;

};

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_DX11_TEXTURE2D_HPP_ */
