#include "Model.hpp"

#include <vector>
#include <unordered_set>
#include <cstdint>

#include "coconut-tools/logger.hpp"

#include <DirectXMath.h>

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;

namespace /* anonymous */ {

CT_LOGGER_CATEGORY("COCONUT.PULP.RENDERER.MODEL");

const milk::math::Vector3d NEEDS_CALCULATION(0.0f, 0.0f, 0.0f);

} // anonymous namespace

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
	currentVertexData_.normal_ = normal;
}

void Model::ModelDataListener::setVertexNormalNeedsCalculation() {
	currentVertexData_.normal_ = NEEDS_CALCULATION;
}

void Model::ModelDataListener::setMaterialName(const std::string& materialName) {
	if (!model_.materialLibrary_.has(materialName)) {
		throw material::MaterialLibrary::NoSuchMaterialInLibrary(materialName);
	}

	currentVertexData_.materialName_ = materialName;
}

size_t Model::ModelDataListener::endVertex() {
	// TODO: do it a better way (keep vertices in some other form)
	currentGroupData_.vertices.emplace_back(std::make_shared<VertexData>(currentVertexData_));
	return currentGroupData_.vertices.size() - 1;
}

void Model::ModelDataListener::addIndex(size_t index) {
	currentGroupData_.indices.emplace_back(index);
}

void Model::ModelDataListener::endFace() {
}

void Model::ModelDataListener::endSmoothingGroup(milk::graphics::PrimitiveTopology primitiveTopology) {
	if (!currentGroupData_.indices.empty()) {
		currentGroupData_.primitiveTopology = primitiveTopology;
		currentGroupData_.material = model_.materialLibrary_.get(currentVertexData_.materialName_);

		calculateMissingNormals();

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

void Model::ModelDataListener::calculateMissingNormals() {
	std::unordered_set<size_t> needCalc;

	for (size_t vertexIdx = 0; vertexIdx < currentGroupData_.vertices.size(); ++vertexIdx) {
		if (currentGroupData_.vertices[vertexIdx]->normal() == NEEDS_CALCULATION) {
			needCalc.emplace(vertexIdx);
		}
	}

	if (needCalc.empty()) {
		return;
	}

	CT_LOG_DEBUG << "Calculating group normals";

	assert(currentGroupData_.primitiveTopology == milk::graphics::PrimitiveTopology::TRIANGLE_LIST); // TODO

	std::unordered_multimap<size_t, size_t> affectingFaces;

	for (size_t faceIdx = 0; faceIdx < currentGroupData_.indices.size(); faceIdx += 3) {
		assert(faceIdx + 2 < currentGroupData_.indices.size());
		for (size_t faceVertexIdx = faceIdx; faceVertexIdx < faceIdx + 3; ++faceVertexIdx) {
			const auto vertexIdx = currentGroupData_.indices[faceVertexIdx];
			if (needCalc.count(vertexIdx)) {
				affectingFaces.emplace(std::make_pair(vertexIdx, faceIdx));
			}
		}
	}

	std::unordered_map<size_t, milk::math::Vector3d> faceNormals;

	for (auto vertexIdx : needCalc) {
		auto range = affectingFaces.equal_range(vertexIdx);

		milk::math::Vector3d normal(0.0f, 0.0f, 0.0f); // TODO: default vector initialisation to 0?

		CT_LOG_TRACE << "Calculating normal for vertex " << vertexIdx << ": " << currentGroupData_.vertices[vertexIdx]->position();

		while (range.first != range.second) {
			const auto faceIdx = range.first->second;
			if (faceNormals.count(faceIdx) == 0) {
				// TODO: kolejnosc
				milk::math::Vector3d a =
					currentGroupData_.vertices[currentGroupData_.indices[faceIdx + 1]]->position() -
					currentGroupData_.vertices[currentGroupData_.indices[faceIdx]]->position();
				milk::math::Vector3d b =
					currentGroupData_.vertices[currentGroupData_.indices[faceIdx + 2]]->position() -
					currentGroupData_.vertices[currentGroupData_.indices[faceIdx]]->position();

				CT_LOG_TRACE << "Face vectors: " << a << " x " << b << " = " << a.cross(b);

				faceNormals.insert(std::make_pair(
					faceIdx,
					a.cross(b)
					));
			}

			normal += faceNormals[faceIdx];
			
			CT_LOG_TRACE << "Temporary normal with face " << (faceIdx / 3) << " is " << normal;

			++range.first;
		}

		const auto length = normal.length();  // TODO: this should be a vector function (normalise)
		if (length > 0.0f) {
			normal /= normal.length();
		}
		
		CT_LOG_TRACE << "Final normal is " << normal;

		dynamic_cast<VertexData&>(*currentGroupData_.vertices[vertexIdx]).normal_ = normal;
	}
}
