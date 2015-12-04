#include "ImageLoader.hpp"

#include <algorithm>

#include <coconut-tools/logger.hpp>

#include "coconut/milk/system/WindowsError.hpp"

CT_LOGGER_CATEGORY("COCONUT.MILK.GRAPHICS");

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::graphics;

namespace /* anonymous */ {

const std::pair<WICPixelFormatGUID, PixelFormat> WIC_TO_PIXEL_FORMAT[] = {
	std::make_pair(GUID_WICPixelFormat32bppRGBA, PixelFormat::R8G8B8A8_UNORM),
	std::make_pair(GUID_WICPixelFormat32bppBGRA, PixelFormat::B8G8R8A8_UNORM),
};

const size_t WIC_TO_PIXEL_FORMAT_COUNT = sizeof(WIC_TO_PIXEL_FORMAT) / sizeof(decltype(WIC_TO_PIXEL_FORMAT[0]));

const std::pair<WICPixelFormatGUID, WICPixelFormatGUID> REQUIRED_FORMAT_CONVERSIONS[] = {
	std::make_pair(GUID_WICPixelFormat32bppPBGRA, GUID_WICPixelFormat32bppRGBA),
	std::make_pair(GUID_WICPixelFormat24bppBGR, GUID_WICPixelFormat32bppRGBA),
};

const size_t REQUIRED_FORMAT_CONVERSIONS_COUNT = sizeof(REQUIRED_FORMAT_CONVERSIONS) / sizeof(decltype(REQUIRED_FORMAT_CONVERSIONS[0]));

PixelFormat wicToPixelFormat(const WICPixelFormatGUID& wicPixelFormat) {
	auto pixelFormatIt = std::find_if(
		WIC_TO_PIXEL_FORMAT,
		WIC_TO_PIXEL_FORMAT + WIC_TO_PIXEL_FORMAT_COUNT,
		[&wicPixelFormat](const decltype(WIC_TO_PIXEL_FORMAT[0])& rhs) {
			return std::memcmp(&wicPixelFormat, &rhs.first, sizeof(WICPixelFormatGUID)) == 0;
		}
		);

	if (pixelFormatIt == WIC_TO_PIXEL_FORMAT + WIC_TO_PIXEL_FORMAT_COUNT) {
		throw std::runtime_error("Unknown pixel format"); // TODO: custom exception
	}

	return pixelFormatIt->second;
}

size_t bitsPerPixel(IWICImagingFactory& imagingFactory, WICPixelFormatGUID wicPixelFormat) {
	system::COMWrapper<IWICComponentInfo> componentInfo;

	system::checkWindowsCall(
		imagingFactory.CreateComponentInfo(wicPixelFormat, &componentInfo.get()),
		"Failed to create component info" // TODO: describe
		);

	system::COMWrapper<IWICPixelFormatInfo> pixelFormatInfo;
	system::checkWindowsCall(
		componentInfo->QueryInterface(__uuidof(IWICPixelFormatInfo), reinterpret_cast<void**>(&pixelFormatInfo.get())),
		"Failed to query pixel format type"
		);

	UINT bpp;
	system::checkWindowsCall(
		pixelFormatInfo->GetBitsPerPixel(&bpp),
		"Failed to retrieve bits per pixel"
		);

	return static_cast<size_t>(bpp);
}

WICPixelFormatGUID targetWicPixelFormat(const WICPixelFormatGUID& source) {
	auto convertIt = std::find_if(
		REQUIRED_FORMAT_CONVERSIONS,
		REQUIRED_FORMAT_CONVERSIONS + REQUIRED_FORMAT_CONVERSIONS_COUNT,
		[&source](const decltype(REQUIRED_FORMAT_CONVERSIONS[0])& rhs) {
		return std::memcmp(&source, &rhs.first, sizeof(WICPixelFormatGUID)) == 0;
	}
	);

	if (convertIt == REQUIRED_FORMAT_CONVERSIONS + REQUIRED_FORMAT_CONVERSIONS_COUNT) {
		return source;
	} else {
		return convertIt->second;
	}
}

void convert(
	IWICImagingFactory& imagingFactory,
	system::COMWrapper<IWICBitmapFrameDecode> frame,
	const WICPixelFormatGUID& targetPixelFormat,
	size_t rowPitch,
	size_t bufferSize,
	std::uint8_t* pixels
	) {
	system::COMWrapper<IWICFormatConverter> formatConverter;
	system::checkWindowsCall(
		imagingFactory.CreateFormatConverter(&formatConverter.get()),
		"Failed to create a format converter"
		);

	system::checkWindowsCall(
		formatConverter->Initialize(frame.get(), targetPixelFormat, WICBitmapDitherTypeErrorDiffusion, nullptr, 0, WICBitmapPaletteTypeCustom),
		"Failed to initialise the format converter"
		);

	formatConverter->CopyPixels(nullptr, static_cast<UINT>(rowPitch), static_cast<UINT>(bufferSize), pixels);
}

} // anonymous namespace

ImageLoadingError::ImageLoadingError(const boost::filesystem::path& path, const std::string& message) :
	coconut_tools::exceptions::RuntimeError(buildMessage(path, message)),
	path_(path)
{
}

ImageLoadingError::ImageLoadingError(const boost::filesystem::path& path, const std::exception& cause) :
	coconut_tools::exceptions::RuntimeError(buildMessage(path, std::string()), cause),
	path_(path)
{
}

std::string ImageLoadingError::buildMessage(const boost::filesystem::path& path, const std::string& message) {
	std::string result = "Failed to load image \"" + path.string() + "\"";
	if (!message.empty()) {
		result += ": " + message;
	}

	return result;
}

ImageLoader::ImageLoader() {
	system::checkWindowsCall(
		CoCreateInstance(
			CLSID_WICImagingFactory,
			nullptr,
			CLSCTX_INPROC_SERVER,
			IID_IWICImagingFactory,
			reinterpret_cast<LPVOID*>(&imagingFactory_.get())
			),
		"Failed to create an ImagingFactory instance"
		);
}

Image ImageLoader::load(const boost::filesystem::path& path) const {
	try {
		auto extension = path.extension().string();

		CT_LOG_DEBUG << "Loading image " << path.string();

		system::COMWrapper<IWICBitmapDecoder> decoder;
		system::checkWindowsCall(
			imagingFactory_->CreateDecoderFromFilename(
				path.wstring().c_str(),
				nullptr,
				GENERIC_READ,
				WICDecodeMetadataCacheOnDemand,
				&decoder.get()
				),
			"Failed to create a decoder for " + extension
			);

		UINT frameCount;
		system::checkWindowsCall(decoder->GetFrameCount(&frameCount), "Failed to retrieve the image frame count");
		if (frameCount == 0) {
			throw ImageLoadingError(path, "No frames in image");
		}

		system::COMWrapper<IWICBitmapFrameDecode> decodedFrame;
		system::checkWindowsCall(decoder->GetFrame(0, &decodedFrame.get()), "Failed to retrieve the decoded frame");

		UINT width, height;
		system::checkWindowsCall(decodedFrame->GetSize(&width, &height), "Failed to retrieve the decoded frame size");

		auto size = std::make_pair(static_cast<size_t>(width), static_cast<size_t>(height));

		WICPixelFormatGUID wicPixelFormat;
		system::checkWindowsCall(decodedFrame->GetPixelFormat(&wicPixelFormat), "Failed to retrieve the decoded frame pixel format");

		auto convertedWicPixelFormat = targetWicPixelFormat(wicPixelFormat);
		auto texturePixelFormat = wicToPixelFormat(convertedWicPixelFormat);
		auto rowPitch = (width * bitsPerPixel(*imagingFactory_, convertedWicPixelFormat) + 7) / 8;
		auto bufferSize = rowPitch * height;
		auto pixels = std::make_unique<std::uint8_t[]>(rowPitch * height);

		if (std::memcmp(&wicPixelFormat, &convertedWicPixelFormat, sizeof(decltype(wicPixelFormat))) == 0) {
			system::checkWindowsCall(
				decodedFrame->CopyPixels(nullptr, static_cast<UINT>(rowPitch), static_cast<UINT>(bufferSize), pixels.get()),
				"Failed to copy image pixels"
				);
		} else {
			CT_LOG_DEBUG << "Converting the image to a supported format";
			convert(*imagingFactory_, decodedFrame, convertedWicPixelFormat, rowPitch, bufferSize, pixels.get());
		}

		return Image(std::move(pixels), size, rowPitch, texturePixelFormat);
	} catch (const std::exception& e) {
		throw ImageLoadingError(path, e);
	}
}
