#include "Heightmap.hpp"

#include <iterator>
#include <algorithm>

#include <coconut-tools/utils/Range.hpp>

#include "coconut/milk/graphics/Texture2d.hpp"
#include "coconut/milk/graphics/ImageLoader.hpp"
#include "coconut/milk/utils/integralValue.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::world;

namespace /* anonymous */ {

std::tuple<float, float> sampleAt(
	const std::vector<float>& cellHeights,
	size_t columnCount,
	std::int64_t rowIndex,
	std::int64_t columnIndex
	)
{
	if (
		rowIndex < 0 ||
		rowIndex >= cellHeights.size() / columnCount ||
		columnIndex < 0 ||
		columnIndex >= columnCount
		)
	{
		return std::make_tuple(0.0f, 0.0f);
	} else {
		return std::make_tuple(cellHeights[rowIndex * columnCount + columnIndex], 1.0f);
	}
}

float smoothAt(
	std::vector<float>& cellHeights,
	size_t columnCount,
	std::int64_t rowIndex,
	std::int64_t columnIndex
	)
{
	auto sum = 0.0f;
	auto count = 0.0f;

	for (const auto sampleRowIndex : coconut_tools::range(rowIndex - 1, rowIndex + 2)) {
		for (const auto sampleColumnIndex : coconut_tools::range(columnIndex - 1, columnIndex + 2)) {
			auto height = 0.0f;
			auto presence = 0.0f;
			std::tie(height, presence) = sampleAt(cellHeights, columnCount, sampleRowIndex, sampleColumnIndex);
			sum += height;
			count += presence;
		}
	}

	assert(count > 3.0f);

	return sum / count;
}

void smooth(std::vector<float>& cellHeights, size_t columnCount) {
	auto result = std::vector<float>(cellHeights.size());

	// TODO: could be done on GPU
	for (const auto rowIndex : coconut_tools::range(size_t(0), cellHeights.size() / columnCount)) {
		for (const auto columnIndex : coconut_tools::range(size_t(0), columnCount)) {
			result[rowIndex * columnCount + columnIndex] =
				smoothAt(cellHeights, columnCount, rowIndex, columnIndex);
		}
	}

	cellHeights.swap(result);
}

} // anonymous namespace

Heightmap::Heightmap(milk::graphics::Renderer& graphicsRenderer, const milk::FilesystemContext& fs) {
	// TODO: all is hardcoded!

	// TODO: fix image api. size() makes no sense, rowPitch doesn't tell whether it's in bytes or pixels,
	// PixelFormat should have some easy-to-use sampler.
	const auto image = coconut::milk::graphics::ImageLoader().load(fs, "data/terrain/heightmap.bmp");
	assert(image.pixelFormat() == coconut::milk::graphics::PixelFormat::R8G8B8A8_UNORM ||
		image.pixelFormat() == coconut::milk::graphics::PixelFormat::B8G8R8A8_UNORM); // TODO...

	const auto heightScale = 50.0f;
	cellEdgeLength_ = 1.0f;
	columnCount_ = image.size().first;
	cellHeights_.resize(image.size().first * image.size().second);
	// TODO: make coconut_tools::range usable for different types to avoid boring casts,
	// maybe even make it work for pointers
	for (const auto pixelIndex : coconut_tools::range(size_t(0), cellHeights_.size())) {
		const auto* pixel = image.pixels() + (pixelIndex * 4);
		cellHeights_[pixelIndex] = (static_cast<float>(*pixel) / 255.0f) * heightScale;
	}

	smooth(cellHeights_, columnCount_);

	auto textureConfiguration = milk::graphics::Texture2d::Configuration();
	textureConfiguration.width = image.size().first;
	textureConfiguration.height = image.size().second;
	textureConfiguration.purpose = milk::graphics::Texture2d::CreationPurpose::SHADER_RESOURCE;
	textureConfiguration.pixelFormat = milk::graphics::PixelFormat::R32_FLOAT;
	textureConfiguration.sampleCount = 1;
	textureConfiguration.sampleQuality = 0;
	textureConfiguration.mipLevels = 1; // TODO: 0 or -1 or what
	textureConfiguration.dataRowPitch = columnCount_ * 4u;
	textureConfiguration.allowModifications = false;
	textureConfiguration.allowCPURead = false;
	textureConfiguration.allowGPUWrite = false;
	textureConfiguration.initialData = cellHeights_.data();

	const auto texture = milk::graphics::Texture2d(graphicsRenderer, textureConfiguration);
	textureSRV_ = milk::graphics::ShaderResourceView(graphicsRenderer, texture);

	auto samplerConfiguration = milk::graphics::Sampler::Configuration();
	samplerConfiguration.addressModeU = milk::graphics::Sampler::AddressMode::CLAMP;
	samplerConfiguration.addressModeV = milk::graphics::Sampler::AddressMode::CLAMP;
	samplerConfiguration.addressModeW = milk::graphics::Sampler::AddressMode::CLAMP;
	samplerConfiguration.filter = milk::graphics::Sampler::Filter::MIN_MAG_LINEAR_MIP_POINT;

	sampler_ = milk::graphics::Sampler(graphicsRenderer, samplerConfiguration);
}

float Heightmap::heightAt(float x, float z) const {
	const auto halfWidth = width() * 0.5f;
	const auto halfDepth = depth() * 0.5f;

	const auto topLeftZ = (z + halfDepth) / cellEdgeLength_;
	const auto topLeftX = (x + halfWidth) / cellEdgeLength_;

	const auto topLeftRow = static_cast<std::int64_t>(topLeftZ);
	const auto topLeftColumn = static_cast<std::int64_t>(topLeftX);

	auto average = 0.0f;
	for (const auto rowIndex : coconut_tools::range(topLeftRow, topLeftRow + 2)) {
		for (const auto columnIndex : coconut_tools::range(topLeftColumn, topLeftColumn+ 2)) {
			auto sampleHeight = 0.0f;
			auto presence = false;
			std::tie(sampleHeight, presence) = sampleAt(cellHeights_, columnCount_, rowIndex, columnIndex);
			if (presence) {
				const auto sampleZ = static_cast<float>(rowIndex) * cellEdgeLength_ - halfDepth;
				const auto sampleX = static_cast<float>(columnIndex) * cellEdgeLength_ - halfWidth;

				const auto zInfluence = cellEdgeLength_ - std::max(sampleZ, z) + std::min(sampleZ, z);
				const auto xInfluence = cellEdgeLength_ - std::max(sampleX, x) + std::min(sampleX, x);

				average += zInfluence * xInfluence * sampleHeight;
			}
		}
	}

	return (1.0f / (cellEdgeLength_ * cellEdgeLength_)) * average; // TODO: this won't work on borders?
}
