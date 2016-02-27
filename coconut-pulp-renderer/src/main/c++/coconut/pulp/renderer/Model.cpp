#include "Model.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;

Model::Model(
	const model::Data& data,
	milk::graphics::Device& graphicsDevice,
	const milk::graphics::InputLayoutDescription& inputLayoutDescription
	) {
	for (size_t groupIndex = 0; groupIndex < data.drawGroups.size(); ++groupIndex) {
		drawGroups_.emplace_back(std::make_shared<DrawGroup>(data, groupIndex, graphicsDevice, inputLayoutDescription));
	}
}

void Model::render(milk::graphics::Device& graphicsDevice, RenderingContext renderingContext) {
	renderingContext.model = this;

	for (auto drawGroup : drawGroups_) {
		drawGroup->render(graphicsDevice, renderingContext);
	}
}
