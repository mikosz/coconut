#include "DrawGroup.hpp"

#include <algorithm>
#include <cstdint>
#include <limits>

#include "coconut/milk/graphics/ImageLoader.hpp"

#include "shader/Pass.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;

namespace /* anonymous */ {

class VertexDataAccessor : public milk::graphics::VertexInterface {
public:

	VertexDataAccessor(const model::Data& modelData, size_t groupIndex) :
		modelData_(modelData),
		groupIndex_(groupIndex),
		vertexIndex_(0)
	{
	}

	virtual milk::math::Vector3d position() const {
		const auto positionIndex = modelData_.drawGroups[groupIndex_].vertices[vertexIndex_].positionIndex;
		return modelData_.positions[positionIndex];
	}

	virtual milk::math::Vector2d textureCoordinate() const {
		const auto textureCoordinateIndex =
			modelData_.drawGroups[groupIndex_].vertices[vertexIndex_].textureCoordinateIndex;
		return modelData_.textureCoordinates[textureCoordinateIndex];
	}

	virtual milk::math::Vector3d normal() const {
		const auto normalIndex = modelData_.drawGroups[groupIndex_].vertices[vertexIndex_].normalIndex;
		return modelData_.normals[normalIndex];
	}

	bool valid() const {
		return vertexIndex_ < modelData_.drawGroups[groupIndex_].vertices.size();
	}

	void progress() {
		++vertexIndex_;
	}

private:

	const model::Data& modelData_;

	const size_t groupIndex_;

	size_t vertexIndex_;

};

milk::graphics::Buffer::Configuration vertexBufferConfiguration(
	const model::Data& modelData,
	size_t groupIndex,
	const milk::graphics::InputLayoutDescription& inputLayoutDescription
	) {
	milk::graphics::Buffer::Configuration configuration;

	configuration.allowCPURead = false;
	configuration.allowGPUWrite = false;
	configuration.allowModifications = false;
	configuration.purpose =  milk::graphics::Buffer::CreationPurpose::VERTEX_BUFFER;
	configuration.stride = inputLayoutDescription.vertexSize();
	configuration.size = configuration.stride * modelData.drawGroups[groupIndex].vertices.size();

	return configuration;
}

milk::graphics::Buffer::Configuration indexBufferConfiguration(const model::Data& modelData, size_t groupIndex) {
	milk::graphics::Buffer::Configuration configuration;

	configuration.allowCPURead = false;
	configuration.allowGPUWrite = false;
	configuration.allowModifications = false;
	configuration.purpose =  milk::graphics::Buffer::CreationPurpose::INDEX_BUFFER;
	// TODO: index size should be 2 or 4 depending on max index, this is hardcoded to 4
	configuration.stride = 4;
	configuration.size = configuration.stride * modelData.drawGroups[groupIndex].indices.size();

	return configuration;
}

std::vector<std::uint8_t> vertexBufferData(
	const model::Data& modelData,
	size_t groupIndex,
	const milk::graphics::InputLayoutDescription& inputLayoutDescription
	) {
	const auto& drawGroup = modelData.drawGroups[groupIndex];
	const auto vertexSize = inputLayoutDescription.vertexSize();

	std::vector<std::uint8_t> data;
	data.resize(vertexSize * drawGroup.vertices.size());

	std::uint8_t* target = &data.front();
	for (
		VertexDataAccessor vertexDataAccessor(modelData, groupIndex);
		vertexDataAccessor.valid();
		vertexDataAccessor.progress()
		) {
		inputLayoutDescription.makeVertex(vertexDataAccessor, target);
		target += vertexSize;
	}

	return data;
}

std::vector<std::uint8_t> indexBufferData(const model::Data& modelData, size_t groupIndex) {
	std::vector<std::uint8_t> data;
	data.resize(4 * modelData.drawGroups[groupIndex].indices.size());

	// TODO: index size should be 2 or 4 depending on max index, this is hardcoded to 4
	std::uint32_t* target = reinterpret_cast<std::uint32_t*>(&data.front());
	for (auto index : modelData.drawGroups[groupIndex].indices) {
		*target = static_cast<std::uint32_t>(index);
		++target;
	}

	return data;
}

} /* anonymous namespace */

DrawGroup::DrawGroup(
	const model::Data& modelData,
	size_t groupIndex,
	milk::graphics::Device& graphicsDevice,
	const milk::graphics::InputLayoutDescription& inputLayoutDescription
	) :
	vertexBuffer_(
		graphicsDevice,
		vertexBufferConfiguration(modelData, groupIndex, inputLayoutDescription),
		&vertexBufferData(modelData, groupIndex, inputLayoutDescription).front()
		),
	indexBuffer_(graphicsDevice, indexBufferConfiguration(modelData, groupIndex), &indexBufferData(modelData, groupIndex).front()),
	indexCount_(modelData.drawGroups[groupIndex].indices.size()),
	primitiveTopology_(modelData.drawGroups[groupIndex].primitiveTopology)
{
	const auto& materialData = modelData.drawGroups[groupIndex].material;
	material_.setAmbientColour(materialData.ambientColour);
	material_.setDiffuseColour(materialData.diffuseColour);

	milk::graphics::ImageLoader imageLoader;
	auto image = imageLoader.load(materialData.diffuseMap);
	material_.setDiffuseMap(std::make_unique<milk::graphics::Texture2d>(graphicsDevice, image));

	material_.setSpecularColour(materialData.specularColour);
	material_.setSpecularExponent(materialData.specularExponent);
}

void DrawGroup::render(milk::graphics::Device& graphicsDevice, RenderingContext renderingContext) {
	vertexBuffer_.bind(graphicsDevice, milk::graphics::ShaderType::VERTEX, 0);
	indexBuffer_.bind(graphicsDevice, milk::graphics::ShaderType::PIXEL, 0);

	renderingContext.material = &material_;

	renderingContext.pass->bind(graphicsDevice, renderingContext);
	renderingContext.pass->bind(graphicsDevice, renderingContext);

	graphicsDevice.draw(0, indexCount_, primitiveTopology_);
}
