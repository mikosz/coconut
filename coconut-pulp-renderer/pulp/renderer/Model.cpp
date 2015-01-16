#include "Model.hpp"

#include <cstdint>

#include <DirectXMath.h>

#include "shader/ShaderFactory.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;

Model::Model(milk::graphics::Device& graphicsDevice, model_loader::ModelLoader& loader) {
	DataListener listener(*this, graphicsDevice);
	loader.load(listener);
}

void Model::render(milk::graphics::Device& graphicsDevice, RenderingContext renderingContext) {
	renderingContext.model = this;

	for (auto drawGroup : drawGroupsByMaterial_) {
		drawGroup.second->render(graphicsDevice, renderingContext);
	}
}

Model::DataListener::DataListener(Model& model, milk::graphics::Device& graphicsDevice) :
	graphicsDevice_(graphicsDevice),
	model_(model)
{
}

void Model::DataListener::setVertexPosition(const milk::math::Vector3d& position) {
	currentVertexData_.position_ = position;
}

void Model::DataListener::setVertexTextureCoordinate(const milk::math::Vector2d& textureCoordinate) {
	currentVertexData_.textureCoordinate_ = textureCoordinate;
}

void Model::DataListener::setVertexNormal(const milk::math::Vector3d& normal) {
	// TODO: implement me!
}

void Model::DataListener::setVertexNormalNeedsCalculation() {
	// TODO: implement me!
}

void Model::DataListener::endVertex() {
	// TODO: do it a better way (keep vertices in some other form)
	currentGroupData_.vertices.push_back(milk::graphics::VertexInterfaceSharedPtr(new VertexData(currentVertexData_)));
	currentGroupData_.indices.push_back(currentGroupData_.vertices.size());
}

void Model::DataListener::endFace() {

}

void Model::DataListener::endSmoothingGroup(milk::graphics::PrimitiveTopology primitiveTopology) {
	if (!currentGroupData_.indices.empty()) {
		currentGroupData_.primitiveTopology = primitiveTopology;

		DrawGroupSharedPtr drawGroup(new DrawGroup(graphicsDevice_, currentGroupData_));
		model_.drawGroupsByMaterial_.insert(std::make_pair(currentGroupData_.material, drawGroup));
	}
}

void Model::DataListener::endObject() {
	currentGroupData_.reset();
}

void Model::DataListener::endModel() {
	// TODO: implement me!
}
