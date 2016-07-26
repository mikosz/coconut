#include "Model.hpp"

#include "coconut/milk/graphics/Texture2d.hpp"
#include "coconut/milk/graphics/Sampler.hpp"
#include "coconut/milk/graphics/ImageLoader.hpp"

#include "PhongMaterial.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;

Model::Model(
	const model::Data& data,
	milk::graphics::Renderer& graphicsRenderer,
	const milk::graphics::InputLayoutDescription& inputLayoutDescription,
	MaterialManager& materialManager
	) {
	for (const auto& phongMaterial : data.phongMaterials) {
		ShaderPassType shaderPassType;
		if (phongMaterial.diffuseColour.w() < 1.0f) { // TODO: - epsilon
			shaderPassType = ShaderPassType::TRANSPARENT;
		} else {
			shaderPassType = ShaderPassType::OPAQUE;
		}

		auto material = std::make_unique<PhongMaterial>(shaderPassType);
		material->setAmbientColour(phongMaterial.ambientColour);
		material->setDiffuseColour(phongMaterial.diffuseColour);

		if (!phongMaterial.diffuseMap.empty()) {
			milk::graphics::ImageLoader imageLoader;
			auto diffuseMap = std::make_unique<milk::graphics::Texture2d>(
				graphicsRenderer, imageLoader.load(phongMaterial.diffuseMap));
			material->setDiffuseMap(std::move(diffuseMap));
			material->setDiffuseMapSampler(
				milk::graphics::Sampler(
					graphicsRenderer, phongMaterial.diffuseMapSamplerConfiguration
					)
				); // TODO: use configuration-derived key, store samplers in manager
		}

		material->setSpecularColour(phongMaterial.specularColour);
		material->setSpecularExponent(phongMaterial.specularExponent);

		materialManager.registerMaterial(phongMaterial.name, std::move(material)); // TODO: feed the material manager somewhere else
	}

	for (size_t groupIndex = 0; groupIndex < data.drawGroups.size(); ++groupIndex) {
		drawGroups_.emplace_back(std::make_shared<DrawGroup>(data, groupIndex, graphicsRenderer, inputLayoutDescription, materialManager));
	}
}

void Model::render(CommandBuffer& commandBuffer, PassContext PassContext) {
	PassContext.model = this;

	for (auto drawGroup : drawGroups_) {
		drawGroup->render(commandBuffer, PassContext);
	}
}
