#include "Model.hpp"

#include "coconut/milk/graphics/Texture2d.hpp"
#include "coconut/milk/graphics/ImageLoader.hpp"

#include "PhongMaterial.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;

Model::Model(
	Context& context,
	const model::Data& data,
	milk::graphics::Renderer& graphicsRenderer,
	const milk::graphics::InputLayoutDescription& inputLayoutDescription
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

		milk::graphics::ImageLoader imageLoader;
		auto diffuseMap = std::make_unique<milk::graphics::Texture2d>(
			graphicsRenderer, imageLoader.load(phongMaterial.diffuseMap));
		material->setDiffuseMap(std::move(diffuseMap));

		material->setSpecularColour(phongMaterial.specularColour);
		material->setSpecularExponent(phongMaterial.specularExponent);

		context.materialManager().registerMaterial(phongMaterial.name, std::move(material));
	}

	for (size_t groupIndex = 0; groupIndex < data.drawGroups.size(); ++groupIndex) {
		drawGroups_.emplace_back(std::make_shared<DrawGroup>(context, data, groupIndex, graphicsRenderer, inputLayoutDescription));
	}
}

void Model::render(CommandBuffer& commandBuffer, PassContext PassContext) {
	PassContext.model = this;

	for (auto drawGroup : drawGroups_) {
		drawGroup->render(commandBuffer, PassContext);
	}
}
