#ifndef _COCONUT_MILK_GRAPHICS_DX11_TEXTURE1D_HPP_
#define _COCONUT_MILK_GRAPHICS_DX11_TEXTURE1D_HPP_

#include <d3d11.h>
#include "coconut/milk/system/cleanup-windows-macros.hpp"

#include "coconut/milk/system/COMWrapper.hpp"

#include "PixelFormat.hpp"
#include "Texture.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class Renderer;
class Image;

class Texture1d : public Texture {
public:

	struct Configuration {

		size_t width;

		size_t arraySize;

		size_t mipLevels;

		PixelFormat pixelFormat;

		bool allowModifications;

		bool allowCPURead;

		bool allowGPUWrite;

		coconut_tools::Mask<CreationPurpose> purposeFlags;

		const void* initialData;

		Configuration() {
			std::memset(this, 0, sizeof(decltype(*this))); // TODO: TEMP TEMP TEMP
			mipLevels = 1;
			arraySize = 1;
		}

	};

	Texture1d() {
	}

	Texture1d(Renderer& renderer, const Configuration& configuration);

	Texture1d(Renderer& renderer, const Image& image);

	Texture1d(system::COMWrapper<ID3D11Texture1D> texture)
	{
		resource_.reset(texture.get());
	}

	void initialise(Renderer& renderer, const Configuration& configuration);

	void initialise(
		Renderer& renderer,
		coconut_tools::Mask<CreationPurpose> purposeFlags,
		system::COMWrapper<ID3D11Texture1D> texture
	);

};

CT_MAKE_POINTER_DEFINITIONS(Texture1d);

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_DX11_TEXTURE1D_HPP_ */
