#include "PixelFormat.hpp"

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
	case PixelFormat::R8G8B8A8_UNORM:
		return 4;
	case PixelFormat::B8G8R8A8_UNORM:
		return 4;
	case PixelFormat::R32_FLOAT:
		return 4;
	default:
		throw std::runtime_error("Unknown format size");
	}
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

