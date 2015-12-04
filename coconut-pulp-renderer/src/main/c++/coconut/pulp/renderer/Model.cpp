#include "Model.hpp"

#include <cstdint>

#include <DirectXMath.h>

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;

Model::Model(
	milk::graphics::Device& graphicsDevice,
	model_loader::ModelLoader& loader,
	const milk::graphics::InputLayoutDescription& inputLayoutDescription
	) {
	ModelDataListener listener(*this, graphicsDevice, inputLayoutDescription);
	loader.load(listener, graphicsDevice);
}

void Model::render(milk::graphics::Device& graphicsDevice, RenderingContext renderingContext) {
	renderingContext.model = this;

	for (auto drawGroup : drawGroupsByMaterial_) {
		drawGroup.second->render(graphicsDevice, renderingContext);
	}
}

Model::ModelDataListener::ModelDataListener(Model& model, milk::graphics::Device& graphicsDevice, const milk::graphics::InputLayoutDescription& inputLayoutDescription) :
	graphicsDevice_(graphicsDevice),
	model_(model)
{
	currentGroupData_.inputLayout = &inputLayoutDescription; // TODO: i dont like this
}

void Model::ModelDataListener::setMaterialLibrary(material::MaterialLibrary&& materialLibrary) {
	model_.materialLibrary_ = std::move(materialLibrary);
}

void Model::ModelDataListener::setVertexPosition(const milk::math::Vector3d& position) {
	currentVertexData_.position_ = position;
}

void Model::ModelDataListener::setVertexTextureCoordinate(const milk::math::Vector2d& textureCoordinate) {
	currentVertexData_.textureCoordinate_ = textureCoordinate;
}

void Model::ModelDataListener::setVertexNormal(const milk::math::Vector3d& normal) {
	// TODO: implement me!
}

void Model::ModelDataListener::setVertexNormalNeedsCalculation() {
	// TODO: implement me!
}

void Model::ModelDataListener::setMaterialName(const std::string& materialName) {
	if (!model_.materialLibrary_.has(materialName)) {
		throw material::MaterialLibrary::NoSuchMaterialInLibrary(materialName);
	}

	currentVertexData_.materialName_ = materialName;
}

void Model::ModelDataListener::endVertex() {
	// TODO: do it a better way (keep vertices in some other form)
	currentGroupData_.vertices.push_back(milk::graphics::VertexInterfaceSharedPtr(new VertexData(currentVertexData_)));
	currentGroupData_.indices.push_back(currentGroupData_.vertices.size() - 1);
}

void Model::ModelDataListener::endFace() {
}

void Model::ModelDataListener::endSmoothingGroup(milk::graphics::PrimitiveTopology primitiveTopology) {
	if (!currentGroupData_.indices.empty()) {
		currentGroupData_.primitiveTopology = primitiveTopology;
		currentGroupData_.material = model_.materialLibrary_.get(currentVertexData_.materialName_);

		DrawGroupSharedPtr drawGroup(new DrawGroup(graphicsDevice_, currentGroupData_));
		model_.drawGroupsByMaterial_.insert(std::make_pair(currentGroupData_.material, drawGroup));
	}
}

void Model::ModelDataListener::endObject() {
	currentGroupData_.reset();
}

void Model::ModelDataListener::endModel() {
	// TODO: implement me!
}
