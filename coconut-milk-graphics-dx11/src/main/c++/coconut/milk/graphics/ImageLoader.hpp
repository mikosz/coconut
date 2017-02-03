#ifndef _COCONUT_MILK_GRAPHICS_DX11_IMAGELOADER_HPP_
#define _COCONUT_MILK_GRAPHICS_DX11_IMAGELOADER_HPP_

#include <vector>

#include <wincodec.h>
#include "coconut/milk/system/cleanup-windows-macros.hpp"
#pragma comment(lib, "windowscodecs.lib")

#include <coconut-tools/exceptions/RuntimeError.hpp>

#include "coconut/milk/fs.hpp"

#include "coconut/milk/system/COMWrapper.hpp"

#include "coconut/milk/math/Vector.hpp"

#include "PixelFormat.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class Image { // TODO: extract to a separate file
public:

	using Dimensions = std::pair<size_t, size_t>; // TODO: find a better type

	const std::uint8_t* pixels() const {
		return pixels_.get();
	}

	Dimensions size() const {
		return size_;
	}

	size_t rowPitch() const {
		return rowPitch_;
	}

	PixelFormat pixelFormat() const {
		return pixelFormat_;
	}

private:

	Image(std::unique_ptr<std::uint8_t[]> pixels, Dimensions size, size_t rowPitch, PixelFormat pixelFormat) :
		pixels_(std::move(pixels)),
		size_(size),
		rowPitch_(rowPitch),
		pixelFormat_(pixelFormat)
	{
	}

	std::unique_ptr<std::uint8_t[]> pixels_;

	Dimensions size_;

	size_t rowPitch_;

	PixelFormat pixelFormat_;

	friend class ImageLoader;

};

class ImageLoadingError : public coconut_tools::exceptions::RuntimeError {
public:

	ImageLoadingError(const AbsolutePath& path, const std::string& message);

	ImageLoadingError(const AbsolutePath& path, const std::exception& cause);

	const std::string& name() const noexcept override {
		static const std::string NAME = "ImageLoadingError";
		return NAME;
	}

	const AbsolutePath& path() const {
		return path_;
	}

private:

	AbsolutePath path_;

	static std::string buildMessage(const AbsolutePath& path, const std::string& message);

};

class ImageLoader {
public:

	ImageLoader();

	Image load(const FilesystemContext& filesystemContext, const Path& path) const;

private:

	system::COMWrapper<IWICImagingFactory> imagingFactory_;

};

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_DX11_IMAGELOADER_HPP_ */
