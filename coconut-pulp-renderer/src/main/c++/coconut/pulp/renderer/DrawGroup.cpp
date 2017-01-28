#include "DrawGroup.hpp"

#include <algorithm>
#include <cstdint>
#include <limits>
#include <memory>

#include "coconut/milk/graphics/ImageLoader.hpp"

#include "shader/Pass.hpp"
#include "CommandBuffer.hpp"
#include "DrawCommand.hpp"

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
	const milk::graphics::InputLayoutDescription& inputLayoutDescription,
	milk::graphics::InputLayoutDescription::SlotType inputSlotType
	) {
	milk::graphics::Buffer::Configuration configuration;

	configuration.allowCPURead = false;
	configuration.allowGPUWrite = false;
	configuration.allowModifications = false;
	configuration.stride = inputLayoutDescription.vertexSize(inputSlotType);
	configuration.size = configuration.stride * modelData.drawGroups[groupIndex].vertices.size();

	return configuration;
}

milk::graphics::Buffer::Configuration indexBufferConfiguration(const model::Data& modelData, size_t groupIndex) {
	milk::graphics::Buffer::Configuration configuration;

	configuration.allowCPURead = false;
	configuration.allowGPUWrite = false;
	configuration.allowModifications = false;

	const auto& indices = modelData.drawGroups[groupIndex].indices;
	assert(!indices.empty());
	size_t maxIndex = *std::max_element(indices.begin(), indices.end());
	const auto max2ByteIndex = std::numeric_limits<std::uint16_t>::max();
	configuration.stride = (maxIndex <= max2ByteIndex ? 2 : 4);

	configuration.size = configuration.stride * modelData.drawGroups[groupIndex].indices.size();

	return configuration;
}

std::vector<std::uint8_t> vertexBufferData(
	const model::Data& modelData,
	size_t groupIndex,
	const milk::graphics::InputLayoutDescription& inputLayoutDescription,
	milk::graphics::InputLayoutDescription::SlotType inputSlotType
	) {
	const auto& drawGroup = modelData.drawGroups[groupIndex];
	const auto vertexSize = inputLayoutDescription.vertexSize(inputSlotType);

	std::vector<std::uint8_t> data;
	data.resize(vertexSize * drawGroup.vertices.size());

	auto* target = data.data();
	for (
		VertexDataAccessor vertexDataAccessor(modelData, groupIndex);
		vertexDataAccessor.valid();
		vertexDataAccessor.progress()
		) {
		inputLayoutDescription.makeVertex(vertexDataAccessor, target, inputSlotType);
		target += vertexSize;
	}

	return data;
}

std::vector<std::uint8_t> indexBufferData(const model::Data& modelData, size_t groupIndex) {
	const auto& indices = modelData.drawGroups[groupIndex].indices;

	// TODO: duplicated code and calculations with indexBufferConfiguration
	const auto maxIndex = *std::max_element(indices.begin(), indices.end());
	const auto max2ByteIndex = std::numeric_limits<std::uint16_t>::max();
	const auto stride = (maxIndex <= max2ByteIndex ? 2 : 4);

	std::vector<std::uint8_t> data;
	data.resize(stride * indices.size());

	if (stride == 2) {
		std::transform(
			indices.begin(),
			indices.end(),
			reinterpret_cast<std::uint16_t*>(data.data()),
			[](size_t index) { return static_cast<std::uint16_t>(index); }
			);
	} else {
		std::transform(
			indices.begin(),
			indices.end(),
			reinterpret_cast<std::uint32_t*>(data.data()),
			[](size_t index) { return static_cast<std::uint32_t>(index); }
		);
	}

	return data;
}

} /* anonymous namespace */

DrawGroup::DrawGroup(
	const model::Data& modelData,
	size_t groupIndex,
	milk::graphics::Renderer& graphicsRenderer,
	const milk::graphics::InputLayoutDescription& inputLayoutDescription,
	const MaterialManager& materialManager
	) :
	material_(materialManager.get(modelData.drawGroups[groupIndex].materialId)),
	rasteriser_(graphicsRenderer, modelData.rasteriserConfiguration),
	vertexBuffer_(
		graphicsRenderer,
		vertexBufferConfiguration(modelData, groupIndex, inputLayoutDescription,
			milk::graphics::InputLayoutDescription::SlotType::PER_VERTEX_DATA),
		&vertexBufferData(modelData, groupIndex, inputLayoutDescription,
			milk::graphics::InputLayoutDescription::SlotType::PER_VERTEX_DATA).front()
		),
	instanceDataBuffer_(
		graphicsRenderer,
		vertexBufferConfiguration(modelData, groupIndex, inputLayoutDescription,
			milk::graphics::InputLayoutDescription::SlotType::PER_INSTANCE_DATA),
		&vertexBufferData(modelData, groupIndex, inputLayoutDescription,
			milk::graphics::InputLayoutDescription::SlotType::PER_INSTANCE_DATA).front()
		),
	indexBuffer_(
		graphicsRenderer,
		indexBufferConfiguration(modelData, groupIndex),
		&indexBufferData(modelData, groupIndex).front()
		),
	indexCount_(modelData.drawGroups[groupIndex].indices.size()),
	primitiveTopology_(modelData.drawGroups[groupIndex].primitiveTopology)
{
}

void DrawGroup::render(CommandBuffer& commandBuffer, PassContext passContext) {
	auto* pass = passContext.getPass(material_->shaderPassType());
	if (pass) {
		auto drawCommand = std::make_unique<DrawCommand>(); // TODO: these need to be created in a separate class and buffered

		drawCommand->setRasteriser(&rasteriser_);

		passContext.material = material_.get();

		drawCommand->setInputLayout(&pass->inputLayout());
		drawCommand->setVertexShader(&pass->vertexShader().shaderData());
		pass->vertexShader().bind(*drawCommand, passContext);
		drawCommand->setPixelShader(&pass->pixelShader().shaderData());
		pass->pixelShader().bind(*drawCommand, passContext);

		drawCommand->setVertexBuffer(&vertexBuffer_);
		drawCommand->setIndexBuffer(&indexBuffer_);
		drawCommand->setIndexCount(indexCount_);
		drawCommand->setPrimitiveTopology(primitiveTopology_);

		drawCommand->setRenderTarget(passContext.backBuffer); // TODO
		drawCommand->setDepthStencil(passContext.screenDepthStencil); // TODO
		drawCommand->setViewport(passContext.viewport); // TODO

		drawCommand->setInstanceCount(2);

		commandBuffer.add(std::move(drawCommand));
	}
}
