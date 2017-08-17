#include "Windmap.hpp"

#include <vector>

#include <coconut-tools/utils/Range.hpp>

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::world;

namespace /* anonymous */ {

const auto POWER_SAMPLES = 1024u;

} // anonymous namespace

renderer::shader::ReflectiveInterface<Windmap>::ReflectiveInterface() {
	emplaceMethod("windDir", [](const Windmap& windmap) { return windmap.windDir_; });
	emplaceMethod("baseIntensity", [](const Windmap& windmap) { return windmap.baseIntensity_; });
	emplaceMethod("intensityAmplitude", [](const Windmap& windmap) { return windmap.intensityAmplitude_; });
	emplaceMethod("powerTexture", [](const Windmap& windmap) { return &windmap.powerTexture_; });
	emplaceMethod("secondaryPowerTexture", [](const Windmap& windmap) { return &windmap.secondaryPowerTexture_; });
	emplaceMethod("sampler", [](const Windmap& windmap) { return &windmap.sampler_; });
}

Windmap::Windmap(milk::graphics::Renderer& graphicsRenderer) :
	perlin_(42u),
	windDir_(1.0f, 0.0f),
	baseIntensity_(0.1f),
	intensityAmplitude_(0.1f),
	textureOffset_(0.0f)
{
	// TODO: a lot of this is hard-coded and temp
	auto textureConfiguration = milk::graphics::Texture1d::Configuration();
	textureConfiguration.width = POWER_SAMPLES;
	textureConfiguration.pixelFormat = milk::graphics::PixelFormat::R32_FLOAT;
	textureConfiguration.allowModifications = true;
	textureConfiguration.allowCPURead = false;
	textureConfiguration.allowGPUWrite = false;
	textureConfiguration.purposeFlags =
		coconut_tools::Mask<milk::graphics::Texture::CreationPurpose>() |
		milk::graphics::Texture::CreationPurpose::SHADER_RESOURCE;
	textureConfiguration.initialData = nullptr;

	powerTexture_ = milk::graphics::Texture1d(graphicsRenderer, textureConfiguration);
	secondaryPowerTexture_ = milk::graphics::Texture1d(graphicsRenderer, textureConfiguration);

	auto samplerConfiguration = milk::graphics::Sampler::Configuration();
	samplerConfiguration.addressModeU = milk::graphics::Sampler::AddressMode::CLAMP;
	samplerConfiguration.addressModeV = milk::graphics::Sampler::AddressMode::CLAMP;
	samplerConfiguration.addressModeW = milk::graphics::Sampler::AddressMode::CLAMP;
	samplerConfiguration.filter = milk::graphics::Sampler::Filter::MIN_MAG_MIP_LINEAR;

	sampler_ = milk::graphics::Sampler(graphicsRenderer, samplerConfiguration); // TODO: api - initialise, or copy
}

void Windmap::update(milk::graphics::Renderer& graphicsRenderer, std::chrono::milliseconds dt)
{
	auto data = graphicsRenderer.lock(powerTexture_, milk::graphics::Renderer::LockPurpose::WRITE_DISCARD);
	auto* samplePtr = reinterpret_cast<float*>(data.get());

	textureOffset_ += baseIntensity_ * static_cast<float>(dt.count()) / 1000.0f;

	for (const auto i : coconut_tools::range<size_t>(0, POWER_SAMPLES)) {
		const auto x = textureOffset_ + static_cast<float>(i) / static_cast<float>(POWER_SAMPLES);

		samplePtr[i] = perlin_.sample({ x, 0.33f, 0.33f });
	}
}
