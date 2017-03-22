#include "ResourceFactory.hpp"

#include <vector>

#include <boost/algorithm/string/join.hpp>

#include <coconut-tools/logger.hpp>

#include "coconut/milk/utils/sliceIdentifier.hpp"

#include "../PassContext.hpp"
#include "../Material.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;
using namespace coconut::pulp::renderer::shader;

namespace /* anonymous */ {

CT_LOGGER_CATEGORY("COCONUT.PULP.RENDERER.SHADER.RESOURCE_FACTORY");

std::unique_ptr<Resource> createDiffuseMap(milk::graphics::ShaderType shaderType, size_t slot) {
	return std::make_unique<TextureResource>(
		[](const PassContext& passContext) -> const milk::graphics::Texture* {
			static const auto DIFFUSE_MAP_PROPERTY = mesh::MaterialConfiguration::DIFFUSE_MAP_TEXTURE;
			if (passContext.material->hasTexture(DIFFUSE_MAP_PROPERTY)) {
				return &std::get<milk::graphics::Texture2d>(passContext.material->texture(DIFFUSE_MAP_PROPERTY));
			} else {
				return nullptr;
			}
		},
		shaderType,
		slot
		);
}

std::unique_ptr<Resource> createDiffuseMapSampler(milk::graphics::ShaderType shaderType, size_t slot) {
	return std::make_unique<SamplerResource>(
		[](const PassContext& passContext) -> const milk::graphics::Sampler* {
			static const auto DIFFUSE_MAP_PROPERTY = mesh::MaterialConfiguration::DIFFUSE_MAP_TEXTURE;
			if (passContext.material->hasTexture(DIFFUSE_MAP_PROPERTY)) {
				return &std::get<milk::graphics::Sampler>(passContext.material->texture(DIFFUSE_MAP_PROPERTY));
			} else {
				return nullptr;
			}
		},
		shaderType,
		slot
		);
}

} // anonymous namespace

detail::ResourceCreator::ResourceCreator() {
	registerBuiltins();
}

auto detail::ResourceCreator::doCreate(
	const std::string& id,
	milk::graphics::ShaderType shaderType,
	size_t slot
	) -> Instance
{
	CT_LOG_INFO << "Creating resource " << id << " at slot " << slot << " of " << shaderType;

	const auto slices = milk::utils::sliceIdentifier<std::vector>(id);
	const auto unifiedId = boost::join(slices, "_");

	CT_LOG_DEBUG << "Identifier converted to " << unifiedId;

	// TODO: problem here is that we call the superclass going around the factory type
	// we could have params "diffuseMap" and "diffuse_map" and world would be created twice
	return Super::doCreate(unifiedId, std::move(shaderType), std::move(slot));
}

void detail::ResourceCreator::registerBuiltins() {
	// phong material
	registerCreator("diffuse_map", &createDiffuseMap);
	registerCreator("diffuse_map_sampler_state", &createDiffuseMapSampler);
}
