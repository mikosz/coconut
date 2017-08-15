#include "Windmap.hpp"

#include <vector>

#include <coconut-tools/utils/Range.hpp>

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::world;

renderer::shader::ReflectiveInterface<Windmap>::ReflectiveInterface() {
	emplaceMethod("primaryDir", [](const Windmap& windmap) { return windmap.primaryDir_; });
	emplaceMethod("secondaryDir", [](const Windmap& windmap) { return windmap.secondaryDir_; });
	emplaceMethod("texcoordOffset", [](const Windmap& windmap) { return windmap.texcoordOffset_; });
	emplaceMethod("texture", [](const Windmap& windmap) { return &windmap.texture_; });
	emplaceMethod("sampler", [](const Windmap& windmap) { return &windmap.sampler_; });
}

Windmap::Windmap(milk::graphics::Renderer& graphicsRenderer, size_t width, size_t height) :
	width_(width),
	height_(height),
	primaryDir_(1.0f, 0.0f),
	secondaryDir_(0.0f, 0.2f),
	perlin_(42u)
{
	// TODO: a lot of this is hard-coded and temp

	auto windData = std::vector<float>(2 * width_ * height_);
	for (const auto rowIdx : coconut_tools::range<size_t>(0, height_)) {
		for (const auto colIdx : coconut_tools::range<size_t>(0, width_)) {
			const auto primaryIdx = rowIdx * width + colIdx;
			const auto secondaryIdx = primaryIdx + 1;

			const auto x = static_cast<float>(colIdx) / static_cast<float>(width_);
			const auto y = static_cast<float>(rowIdx) / static_cast<float>(height_);

			windData[primaryIdx] = perlin_.sample({ x, y, 0.0f });
			windData[secondaryIdx] = perlin_.sample({ x, y, 0.1f });
		}
	}

	auto textureConfiguration = milk::graphics::Texture2d::Configuration();
	textureConfiguration.width = width_;
	textureConfiguration.height = height_;
	textureConfiguration.pixelFormat = milk::graphics::PixelFormat::R32G32_FLOAT;
	textureConfiguration.allowModifications = false;
	textureConfiguration.allowCPURead = false;
	textureConfiguration.allowGPUWrite = false;
	textureConfiguration.purposeFlags =
		coconut_tools::Mask<milk::graphics::Texture::CreationPurpose>() |
		milk::graphics::Texture::CreationPurpose::SHADER_RESOURCE;
	textureConfiguration.initialData = windData.data();

	texture_ = milk::graphics::Texture2d(graphicsRenderer, textureConfiguration);

	auto samplerConfiguration = milk::graphics::Sampler::Configuration();
	samplerConfiguration.addressModeU = milk::graphics::Sampler::AddressMode::WRAP;
	samplerConfiguration.addressModeV = milk::graphics::Sampler::AddressMode::WRAP;
	samplerConfiguration.addressModeW = milk::graphics::Sampler::AddressMode::WRAP;
	samplerConfiguration.filter = milk::graphics::Sampler::Filter::MIN_MAG_MIP_LINEAR;

	sampler_ = milk::graphics::Sampler(graphicsRenderer, samplerConfiguration); // TODO: api - initialise, or copy
}

void Windmap::update(std::chrono::milliseconds /*dt*/)
{
}
