#include "Windmap.hpp"

#include <coconut-tools/utils/Range.hpp>

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::world;

renderer::shader::ReflectiveInterface<Windmap>::ReflectiveInterface() {
	emplaceMethod("texture", [](const Windmap& windmap) { return &windmap.texture_; });
	emplaceMethod("sampler", [](const Windmap& windmap) { return &windmap.sampler_; });
}

Windmap::Windmap(milk::graphics::Renderer& graphicsRenderer, size_t width, size_t height) :
	map_(width * height, Vec2(0.0f, 0.0f)),
	width_(width),
	height_(height)
{
	auto textureConfiguration = milk::graphics::Texture2d::Configuration();
	textureConfiguration.width = width_;
	textureConfiguration.height = height_;
	textureConfiguration.pixelFormat = milk::graphics::PixelFormat::R32G32_FLOAT;
	textureConfiguration.allowModifications = true;
	textureConfiguration.allowCPURead = false;
	textureConfiguration.allowGPUWrite = false;
	textureConfiguration.purposeFlags =
		coconut_tools::Mask<milk::graphics::Texture::CreationPurpose>() |
		milk::graphics::Texture::CreationPurpose::SHADER_RESOURCE;
	textureConfiguration.initialData = map_.data();

	texture_ = milk::graphics::Texture2d(graphicsRenderer, textureConfiguration);

	auto samplerConfiguration = milk::graphics::Sampler::Configuration();
	samplerConfiguration.addressModeU = milk::graphics::Sampler::AddressMode::WRAP;
	samplerConfiguration.addressModeV = milk::graphics::Sampler::AddressMode::WRAP;
	samplerConfiguration.addressModeW = milk::graphics::Sampler::AddressMode::WRAP;
	samplerConfiguration.filter = milk::graphics::Sampler::Filter::MIN_MAG_MIP_LINEAR;

	sampler_ = milk::graphics::Sampler(graphicsRenderer, samplerConfiguration); // TODO: api - initialise, or copy
}

void Windmap::update(
	std::chrono::milliseconds /*dt*/,
	milk::graphics::Renderer& graphicsRenderer
	) {
	updateMap_ = map_;

	for (const auto row : coconut_tools::range<size_t>(0, height_)) {
		for (const auto col : coconut_tools::range<size_t>(0, width_)) {
			const auto& dir = map_[row * width_ + col];

			if (dir.x > 0.0f) {
				if (dir.y > 0.0f) {

				}

				if (dir.y < 0.0f) {

				}
			}

			if (dir.x < 0.0f) {
				if (dir.y > 0.0f) {

				}

				if (dir.y < 0.0f) {

				}
			}

			if (dir.y > 0.0f) {

			}

			if (dir.y < 0.0f) {

			}
		}
	}

	{
		auto lockedData = graphicsRenderer.lock(texture_, milk::graphics::Renderer::LockPurpose::WRITE_DISCARD);
		auto* textureSectors = reinterpret_cast<Vec2*>(lockedData.get());
		std::copy(map_.begin(), map_.end(), textureSectors);
	}
}
