#include "Heightmap.hpp"

#include <iterator>
#include <algorithm>

#include <coconut-tools/utils/Range.hpp>

#include "coconut/milk/graphics/ImageLoader.hpp"
#include "coconut/milk/utils/integralValue.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::world;

Heightmap::Heightmap(milk::graphics::Renderer& graphicsRenderer, const milk::FilesystemContext& fs) {
	// TODO: all is hardcoded!

	// TODO: fix image api. size() makes no sense, rowPitch doesn't tell whether it's in bytes or pixels,
	// PixelFormat should have some easy-to-use sampler.
	const auto image = coconut::milk::graphics::ImageLoader().load(fs, "data/terrain/heightmap.bmp");
	assert(image.pixelFormat() == coconut::milk::graphics::PixelFormat::R8G8B8A8_UNORM); // TODO...

	const auto heightScale = 100.0f;
	cellEdgeLength_ = 1.0f;
	columnCount_ = image.size().first;
	cellHeights_.resize(image.size().first * image.size().second);
	// TODO: make coconut_tools::range usable for different types to avoid boring casts,
	// maybe even make it work for pointers
	for (const auto pixelIndex : coconut_tools::range(size_t(0), cellHeights_.size())) {
		const auto* pixel = image.pixels() + (pixelIndex * 4);
		cellHeights_[pixelIndex] = (static_cast<float>(*pixel) / 255.0f) * heightScale;
	}

	auto textureConfiguration = milk::graphics::Texture2d::Configuration();
	textureConfiguration.width = image.size().first;
	textureConfiguration.height = image.size().second;
	textureConfiguration.purposeFlags =
		milk::utils::integralValue(milk::graphics::Texture2d::CreationPurpose::SHADER_RESOURCE); // TODO: ugh
	textureConfiguration.pixelFormat = milk::graphics::PixelFormat::R32_FLOAT;
	textureConfiguration.sampleCount = 1;
	textureConfiguration.sampleQuality = 0;
	textureConfiguration.mipLevels = 1; // TODO: 0 or -1 or what
	textureConfiguration.dataRowPitch = columnCount_ * 4u;
	textureConfiguration.allowModifications = false;
	textureConfiguration.allowCPURead = false;
	textureConfiguration.allowGPUWrite = false;
	textureConfiguration.initialData = cellHeights_.data();

	texture_.initialise(graphicsRenderer, textureConfiguration);

	auto samplerConfiguration = milk::graphics::Sampler::Configuration();
	samplerConfiguration.addressModeU = milk::graphics::Sampler::AddressMode::CLAMP;
	samplerConfiguration.addressModeV = milk::graphics::Sampler::AddressMode::CLAMP;
	samplerConfiguration.addressModeW = milk::graphics::Sampler::AddressMode::CLAMP;
	samplerConfiguration.filter = milk::graphics::Sampler::Filter::MIN_MAG_LINEAR_MIP_POINT;

	sampler_ = milk::graphics::Sampler(graphicsRenderer, samplerConfiguration); // TODO: api - initialise, or copy
}
