#include "Material.hpp"

#include "coconut/milk/graphics/ImageLoader.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;

namespace /* anonymous */ {

auto loadTextures(
	milk::graphics::Renderer graphicsRenderer,
	const milk::fs::FilesystemContext& filesystemContext,
	mesh::MaterialConfiguration materialConfiguration
	)
{
	auto result = std::unordered_map<std::string, Material::Texture>();
	
	auto imageLoader = milk::graphics::ImageLoader();
	for (auto& textureEntry : materialConfiguration.textures()) {
		auto name = std::move(textureEntry.first);

		auto path = milk::fs::AbsolutePath();
		auto samplerConfiguration = milk::graphics::Sampler::Configuration();
		std::tie(path, samplerConfiguration) = textureEntry.second;

		auto texture = milk::graphics::Texture2d(graphicsRenderer, imageLoader.load(filesystemContext, path));
		auto sampler = milk::graphics::Sampler(graphicsRenderer, samplerConfiguration); // TODO: at the moment
			// we force that a sampler be provided for all textures - this should not be required, as some
			// textures are just provided as memory buffers (correctly?)

		result.emplace(name, std::make_tuple(texture, sampler));
	}

	return result;
}

} // anonymous namespace

Material::Material(
	milk::graphics::Renderer graphicsRenderer,
	shader::PassFactory& passFactory,
	const milk::fs::FilesystemContext& filesystemContext,
	mesh::MaterialConfiguration materialConfiguration
	) :
	passType_(materialConfiguration.passType()),
	pass_(passFactory.create(materialConfiguration.shaderName(), graphicsRenderer, filesystemContext)),
	renderState_(graphicsRenderer, materialConfiguration.renderStateConfiguration()),
	properties_(std::move(materialConfiguration.properties())),
	textures_(loadTextures(graphicsRenderer, filesystemContext, materialConfiguration))
{
}

const Primitive& Material::property(const std::string& key) const {
	auto it = properties_.find(key);
	if (it == properties_.end()) {
		throw NoSuchProperty(key);
	} else {
		return it->second;
	}
}

auto Material::texture(const std::string& key) const -> const Texture& {
	auto it = textures_.find(key);
	if (it == textures_.end()) {
		throw NoSuchProperty(key);
	} else {
		return it->second;
	}
}
