#include "ImageLoader.hpp"

#include <algorithm>
#include <iterator>

#include <DirectXTex/DirectXTex.h>

#include <coconut-tools/logger.hpp>

#include "coconut/milk/system/WindowsError.hpp"

#include "PixelFormat.hpp"

CT_LOGGER_CATEGORY("COCONUT.MILK.GRAPHICS");

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::graphics;

ImageLoadingError::ImageLoadingError(const AbsolutePath& path, const std::string& message) :
	coconut_tools::exceptions::RuntimeError(buildMessage(path, message)),
	path_(path)
{
}

ImageLoadingError::ImageLoadingError(const AbsolutePath& path, const std::exception& cause) :
	coconut_tools::exceptions::RuntimeError(buildMessage(path, std::string()), cause),
	path_(path)
{
}

std::string ImageLoadingError::buildMessage(const AbsolutePath& path, const std::string& message) {
	std::string result = "Failed to load image \"" + path.string() + "\"";
	if (!message.empty()) {
		result += ": " + message;
	}

	return result;
}

Image ImageLoader::load(const FilesystemContext& filesystemContext, const Path& path) const {
	try {
		CT_LOG_DEBUG << "Loading image " << path.string();

		const auto imageData = filesystemContext.load(path);
		const auto extension = path.extension();
		auto metadata = DirectX::TexMetadata();
		auto scratchImage = DirectX::ScratchImage();

		if (extension == ".tga") {
			system::checkWindowsCall(
				DirectX::LoadFromTGAMemory(imageData->data(), imageData->size(), &metadata, scratchImage),
				"Failed to load a targa file"
				);
		} else if (extension == ".dds") {
			system::checkWindowsCall(
				DirectX::LoadFromDDSMemory(
					imageData->data(),
					imageData->size(),
					DirectX::DDS_FLAGS_NONE,
					&metadata,
					scratchImage
					),
				"Failed to load a targa file"
				);
		}

		auto pixels = std::vector<std::uint8_t>();
		pixels.reserve(scratchImage.GetPixelsSize());
		std::copy(
			scratchImage.GetPixels(),
			scratchImage.GetPixels() + scratchImage.GetPixelsSize(),
			std::back_inserter(pixels)
			);

		if (allPixelFormatValues().count(static_cast<PixelFormat>(metadata.format)) == 0) {
			throw std::runtime_error("Unsupported image format");
		}

		return Image(
			std::move(pixels),
			{ metadata.width, metadata.height },
			metadata.width,
			static_cast<PixelFormat>(metadata.format)
			);
	} catch (const std::exception& e) {
		throw ImageLoadingError(path, e);
	}
}
