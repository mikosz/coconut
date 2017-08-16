#include "Windmap.hpp"

#include <vector>

#include <coconut-tools/utils/Range.hpp>

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::world;

renderer::shader::ReflectiveInterface<Windmap>::ReflectiveInterface() {
	emplaceMethod("windDir", [](const Windmap& windmap) { return windmap.windDir_; });
	emplaceMethod("basePower", [](const Windmap& windmap) { return windmap.basePower_; });
	emplaceMethod("texcoordOffset", [](const Windmap& windmap) { return windmap.texcoordOffset_; });
	emplaceMethod("powerTexture", [](const Windmap& windmap) { return &windmap.powerTexture_; });
	emplaceMethod("secondaryPowerTexture", [](const Windmap& windmap) { return &windmap.secondaryPowerTexture_; });
	emplaceMethod("sampler", [](const Windmap& windmap) { return &windmap.sampler_; });
}

Windmap::Windmap(milk::graphics::Renderer& graphicsRenderer) :
	windDir_(1.0f, 0.0f),
	basePower_(1.0f),
	texcoordOffset_(0.0f),
	perlin_(42u)
{
	// TODO: a lot of this is hard-coded and temp

	const auto powerSamples = 1024u;

	auto powerData = std::vector<float>(powerSamples);
	auto secondaryPowerData = std::vector<float>(powerSamples);

	for (const auto i : coconut_tools::range<size_t>(0, powerSamples)) {
		const auto x = static_cast<float>(i) / static_cast<float>(powerSamples);

		powerData[i] = perlin_.sample({ x * 0.1f, 0.33f, 0.33f });
		secondaryPowerData[i] = perlin_.sample({ x, 0.66f, 0.66f });
	}

	auto textureConfiguration = milk::graphics::Texture1d::Configuration();
	textureConfiguration.width = powerSamples;
	textureConfiguration.pixelFormat = milk::graphics::PixelFormat::R32_FLOAT;
	textureConfiguration.allowModifications = true;
	textureConfiguration.allowCPURead = false;
	textureConfiguration.allowGPUWrite = false;
	textureConfiguration.purposeFlags =
		coconut_tools::Mask<milk::graphics::Texture::CreationPurpose>() |
		milk::graphics::Texture::CreationPurpose::SHADER_RESOURCE;
	textureConfiguration.initialData = powerData.data();

	powerTexture_ = milk::graphics::Texture1d(graphicsRenderer, textureConfiguration);

	textureConfiguration.initialData = secondaryPowerData.data();

	secondaryPowerTexture_ = milk::graphics::Texture1d(graphicsRenderer, textureConfiguration);

	auto samplerConfiguration = milk::graphics::Sampler::Configuration();
	samplerConfiguration.addressModeU = milk::graphics::Sampler::AddressMode::CLAMP;
	samplerConfiguration.addressModeV = milk::graphics::Sampler::AddressMode::CLAMP;
	samplerConfiguration.addressModeW = milk::graphics::Sampler::AddressMode::CLAMP;
	samplerConfiguration.filter = milk::graphics::Sampler::Filter::MIN_MAG_MIP_LINEAR;

	sampler_ = milk::graphics::Sampler(graphicsRenderer, samplerConfiguration); // TODO: api - initialise, or copy
}

void Windmap::update(std::chrono::milliseconds /*dt*/)
{
}
