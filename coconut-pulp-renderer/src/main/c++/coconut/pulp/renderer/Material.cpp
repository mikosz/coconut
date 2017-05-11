#include "Material.hpp"

#include "coconut/milk/graphics/ImageLoader.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;

namespace /* anonymous */ {

void bindResourceProperties(
	shader::Properties& shaderProperties,
	milk::graphics::Renderer graphicsRenderer,
	const milk::fs::FilesystemContext& filesystemContext,
	mesh::MaterialConfiguration materialConfiguration
	)
{
	auto imageLoader = milk::graphics::ImageLoader();
	for (auto& textureEntry : materialConfiguration.textures()) {
		auto name = std::move(textureEntry.first);

		auto path = milk::fs::AbsolutePath();
		auto samplerConfiguration = milk::graphics::Sampler::Configuration();
		std::tie(path, samplerConfiguration) = textureEntry.second;

		auto sampler = milk::graphics::Sampler(graphicsRenderer, samplerConfiguration); // TODO: at the moment
			// we force that a sampler be provided for all textures - this should not be required, as some
			// textures are just provided as memory buffers (correctly?)
		shaderProperties.bind(name + "Sampler", std::move(sampler));
		// TODO: if we decide to keep samplers always defined for textures, then lets have a texture type
		// with both texture and sampler specified and name them together. Otherwise should pass name
		// of sampler through materialConfiguration

		auto texture = milk::graphics::Texture2d(graphicsRenderer, imageLoader.load(filesystemContext, path));
		shaderProperties.bind(std::move(name), std::move(texture));
	}
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
	renderState_(graphicsRenderer, materialConfiguration.renderStateConfiguration())
{
	for (const auto entry : materialConfiguration.properties()) {
		shaderProperties_.bind(std::move(entry.first), std::move(entry.second));
	}

	bindResourceProperties(shaderProperties_, graphicsRenderer, filesystemContext, materialConfiguration);
}
