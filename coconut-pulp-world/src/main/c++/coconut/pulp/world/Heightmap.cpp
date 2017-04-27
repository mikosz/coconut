#include "Heightmap.hpp"

#include <iterator>
#include <algorithm>

#include <coconut-tools/utils/Range.hpp>

#include "coconut/milk/graphics/ImageLoader.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::world;

Heightmap::Heightmap(const milk::FilesystemContext& fs) {
	// TODO: all is hardcoded!

	// TODO: fix image api. size() makes no sense, rowPitch doesn't tell whether it's in bytes or pixels,
	// PixelFormat should have some easy-to-use sampler.
	const auto image = coconut::milk::graphics::ImageLoader().load(fs, "data/terrain/heightmap.bmp");
	assert(image.pixelFormat() == coconut::milk::graphics::PixelFormat::R8G8B8A8_UNORM); // TODO...

	const auto heightScale = 100.0f;
	patchScale_ = 1.0f;
	rowPitch_ = image.rowPitch();
	data_.resize((image.size().first * image.size().second) / 4);
	// TODO: make coconut_tools::range usable for different types to avoid boring casts,
	// maybe even make it work for pointers
	for (const auto pixelIndex : coconut_tools::range(size_t(0), data_.size())) {
		const auto* pixel = image.pixels() + (pixelIndex * 4);
		data_[pixelIndex] = (static_cast<float>(*pixel) / 255.0f) * heightScale;
	}

	auto textureConfiguration = milk::graphics::Texture2d::Configuration();
	textureConfiguration.allowCPURead = false;
	textureConfiguration.allowGPUWrite = false;

	//texture_.initialise(, textureConfiguration);
}
