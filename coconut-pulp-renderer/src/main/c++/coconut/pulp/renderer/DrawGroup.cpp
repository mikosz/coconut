#include "DrawGroup.hpp"

#include <algorithm>
#include <cstdint>
#include <limits>
#include <memory>

#include "coconut/milk/graphics/ImageLoader.hpp"

#include "shader/Pass.hpp"
#include "CommandBuffer.hpp"
#include "DrawCommand.hpp"
#include "PassContext.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;

namespace /* anonymous */ {

milk::graphics::Buffer::Configuration vertexBufferConfiguration(
	size_t vertexCount,
	size_t vertexSize
	) {
	milk::graphics::Buffer::Configuration configuration;

	configuration.allowCPURead = false;
	configuration.allowGPUWrite = false;
	configuration.allowModifications = false;
	configuration.stride = vertexSize;
	configuration.size = configuration.stride * vertexCount;

	return configuration;
}

milk::graphics::Buffer::Configuration indexBufferConfiguration(const mesh::Submesh& submesh) {
	milk::graphics::Buffer::Configuration configuration;

	configuration.allowCPURead = false;
	configuration.allowGPUWrite = false;
	configuration.allowModifications = false;

	const auto& indices = submesh.indices();
	assert(!indices.empty());
	size_t maxIndex = *std::max_element(indices.begin(), indices.end());
	const auto max2ByteIndex = std::numeric_limits<std::uint16_t>::max();
	configuration.stride = (maxIndex <= max2ByteIndex ? 2 : 4);

	configuration.size = configuration.stride * submesh.indices().size();

	return configuration;
}

std::vector<std::uint8_t> vertexBufferData(
	const mesh::Submesh& submesh,
	const shader::Input& shaderInput
	)
{
	const auto vertexSize = shaderInput.vertexSize();

	std::vector<std::uint8_t> data;
	data.resize(vertexSize * submesh.vertices().size());

	auto* target = data.data();
	for (const auto& vertex : submesh.vertices())
	{
		shaderInput.writeVertex(target, vertex);
		target += vertexSize;
	}

	return data;
}

std::vector<std::uint8_t> indexBufferData(const mesh::Submesh& submesh) {
	const auto& indices = submesh.indices();

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
	milk::graphics::Renderer& graphicsRenderer,
	const shader::Input& shaderInput,
	const mesh::Submesh& submesh
	) :
	vertexBuffer_(
		graphicsRenderer,
		vertexBufferConfiguration(submesh.vertices().size(), shaderInput.vertexSize()),
		vertexBufferData(submesh, shaderInput).data()
		),
	indexCount_(submesh.indices().size()),
	primitiveTopology_(submesh.primitiveTopology())
{
	if (indexCount_ > 0) {
		indexBuffer_ = milk::graphics::IndexBuffer(
			graphicsRenderer,
			indexBufferConfiguration(submesh),
			&indexBufferData(submesh).front()
			);
	}
}

void DrawGroup::render(CommandBuffer& commandBuffer, PassContext passContext) {
	auto drawCommand = std::make_unique<DrawCommand>(); // TODO: these need to be created in a separate class and buffered

	drawCommand->setRasteriser(passContext.rasteriser);

	drawCommand->setInputLayout(&pass->input().layout());
	drawCommand->setVertexShader(&pass->vertexShader().shaderData());
	pass->vertexShader().bind(*drawCommand, passContext);
	drawCommand->setPixelShader(&pass->pixelShader().shaderData());
	pass->pixelShader().bind(*drawCommand, passContext);

	drawCommand->setVertexBuffer(&vertexBuffer_);
	if (instanceDataBuffer_) {
		drawCommand->setInstanceDataBuffer(instanceDataBuffer_.get_ptr());
	}
	if (indexBuffer_) {
		drawCommand->setIndexBuffer(indexBuffer_.get_ptr());
	}
	drawCommand->setIndexCount(indexCount_);
	drawCommand->setPrimitiveTopology(primitiveTopology_);

	drawCommand->setRenderTarget(passContext.backBuffer); // TODO
	drawCommand->setDepthStencil(passContext.screenDepthStencil); // TODO
	drawCommand->setViewport(passContext.viewport); // TODO

	drawCommand->setInstanceCount(instanceCount_);

	commandBuffer.add(std::move(drawCommand));
}
