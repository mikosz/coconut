#include "PixelFormat.hpp"

#include <algorithm>

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::graphics;

// TODO: temp
size_t graphics::formatSize(PixelFormat format) {
	switch (format) {
	case PixelFormat::R32G32_FLOAT:
		return 2 * 4;
	case PixelFormat::R32G32B32_FLOAT:
		return 3 * 4;
	case PixelFormat::R32G32B32A32_FLOAT:
		return 4 * 4;
	case PixelFormat::R8G8B8A8_UNORM_SRGB:
	case PixelFormat::R8G8B8A8_UNORM:
	case PixelFormat::B8G8R8A8_UNORM:
	case PixelFormat::B8G8R8X8_UNORM:
		return 4;
	case PixelFormat::R32_FLOAT:
		return 4;
	case PixelFormat::BC1_UNORM:
		return 8;
	default:
		throw std::runtime_error("Unknown format size");
	}
}

size_t graphics::formatRowPitch(PixelFormat format, size_t width) {
	switch (format) {
	case PixelFormat::BC1_UNORM:
		return std::max<size_t>(1, (width + 3) / 4) * formatSize(format);
	default:
		return formatSize(format) * width;
	}
}

size_t graphics::formatSlicePitch(PixelFormat format, size_t height, size_t rowPitch) {
	auto verticalBlockCount = size_t(0);

	switch (format) {
	case PixelFormat::BC1_UNORM:
		verticalBlockCount = std::max<size_t>(1, (height + 3) / 4);
		break;
	default:
		verticalBlockCount = height;
		break;
	}

	return verticalBlockCount * rowPitch;
}

const char* const graphics::formatHLSLType(PixelFormat format) {
	switch (format) {
	case PixelFormat::R32G32_FLOAT:
		return "float2";
	case PixelFormat::R32G32B32_FLOAT:
		return "float3";
	case PixelFormat::R32G32B32A32_FLOAT:
		return "float4";
	default:
		throw std::runtime_error("Unknown hlsl type");
	}
}

