#ifndef _COCONUT_MILK_GRAPHICS_DX11_TEXTURE2D_HPP_
#define _COCONUT_MILK_GRAPHICS_DX11_TEXTURE2D_HPP_

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

class Texture2d : public Texture {
public:

	struct Configuration {

		size_t width;

		size_t height;

		size_t arraySize;

		size_t mipLevels;

		PixelFormat pixelFormat;

		size_t sampleCount;

		size_t sampleQuality;

		bool allowModifications;

		bool allowCPURead;

		bool allowGPUWrite;

		CreationPurposeFlag purposeFlags;

		const void* initialData;
		
		size_t dataRowPitch;

		Configuration() {
			std::memset(this, 0, sizeof(decltype(*this))); // TODO: TEMP TEMP TEMP
			mipLevels = 1;
			sampleCount = 1;
			sampleQuality = 0;
			arraySize = 1;
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

	void initialise(Renderer& renderer, CreationPurposeFlag purposeFlags, system::COMWrapper<ID3D11Texture2D> texture);

	void reset();

	ID3D11Resource& internalResource() override {
		return *texture_;
	}

private:

	system::COMWrapper<ID3D11Texture2D> texture_;

};

CCN_MAKE_POINTER_DEFINITIONS(Texture2d);

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_DX11_TEXTURE2D_HPP_ */
