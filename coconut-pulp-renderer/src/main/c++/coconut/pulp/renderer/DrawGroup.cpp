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

milk::graphics::Buffer::Configuration vertexBufferConfiguration(
	const model::Data& modelData,
	size_t groupIndex,
	size_t vertexSize
	) {
	milk::graphics::Buffer::Configuration configuration;

	configuration.allowCPURead = false;
	configuration.allowGPUWrite = false;
	configuration.allowModifications = false;
	configuration.stride = vertexSize;
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
	const shader::Input& shaderInput,
	shader::Input::SlotType inputSlotType
	) {
	const auto& drawGroup = modelData.drawGroups[groupIndex];
	const auto vertexSize = shaderInput.vertexSize(inputSlotType);

	std::vector<std::uint8_t> data;
	data.resize(vertexSize * drawGroup.vertices.size());

	auto* target = data.data();
	for (
		auto vertexIterator = model::Data::VertexIterator(modelData, drawGroup);
		!vertexIterator.atEnd();
		vertexIterator.next()
		)
	{
		shaderInput.writeVertex(target, vertexIterator, inputSlotType);
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
	const shader::Input& shaderInput,
	const MaterialManager& materialManager
	) :
	material_(materialManager.get(modelData.drawGroups[groupIndex].materialId)),
	rasteriser_(graphicsRenderer, modelData.rasteriserConfiguration),
	vertexBuffer_(
		graphicsRenderer,
		vertexBufferConfiguration(modelData, groupIndex,
			shaderInput.vertexSize(shader::Input::SlotType::PER_VERTEX_DATA)),
		&vertexBufferData(modelData, groupIndex, shaderInput,
			shader::Input::SlotType::PER_VERTEX_DATA).front()
		),
	instanceDataBuffer_(
		graphicsRenderer,
		vertexBufferConfiguration(modelData, groupIndex, 
			shaderInput.vertexSize(shader::Input::SlotType::PER_INSTANCE_DATA)),
		&vertexBufferData(modelData, groupIndex, shaderInput,
			shader::Input::SlotType::PER_INSTANCE_DATA).front()
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

		drawCommand->setInputLayout(&pass->input().layout());
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
