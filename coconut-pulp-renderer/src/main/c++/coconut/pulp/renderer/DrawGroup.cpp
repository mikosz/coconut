#include "DrawGroup.hpp"

#include <algorithm>
#include <cstdint>
#include <limits>
#include <memory>

#include "coconut/milk/graphics/ImageLoader.hpp"

#include "shader/Pass.hpp"
#include "CommandBuffer.hpp"
#include "GeometryDrawCommand.hpp"

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
	configuration.stride = inputLayoutDescription.vertexSize();
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
	const milk::graphics::InputLayoutDescription& inputLayoutDescription
	) {
	const auto& drawGroup = modelData.drawGroups[groupIndex];
	const auto vertexSize = inputLayoutDescription.vertexSize();

	std::vector<std::uint8_t> data;
	data.resize(vertexSize * drawGroup.vertices.size());

	auto* target = data.data();
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

milk::graphics::Rasteriser::Configuration rasteriserConfiguration() {
	milk::graphics::Rasteriser::Configuration configuration;

	configuration.cullMode = milk::graphics::Rasteriser::CullMode::NONE;
	configuration.fillMode = milk::graphics::Rasteriser::FillMode::SOLID;
	configuration.frontCounterClockwise = false;

	return configuration;
}

milk::graphics::Sampler::Configuration samplerConfiguration() {
	milk::graphics::Sampler::Configuration configuration;

	configuration.addressModeU = milk::graphics::Sampler::AddressMode::WRAP;
	configuration.addressModeV = milk::graphics::Sampler::AddressMode::WRAP;
	configuration.addressModeW = milk::graphics::Sampler::AddressMode::WRAP;
	configuration.filter = milk::graphics::Sampler::Filter::ANISOTROPIC;

	return configuration;
}

} /* anonymous namespace */

DrawGroup::DrawGroup(
	Context& context,
	const model::Data& modelData,
	size_t groupIndex,
	milk::graphics::Renderer& graphicsRenderer,
	const milk::graphics::InputLayoutDescription& inputLayoutDescription
	) :
	material_(context.materialManager().get(modelData.drawGroups[groupIndex].materialId)),
	vertexBuffer_(
		graphicsRenderer,
		vertexBufferConfiguration(modelData, groupIndex, inputLayoutDescription),
		&vertexBufferData(modelData, groupIndex, inputLayoutDescription).front()
		),
	indexBuffer_(
		graphicsRenderer,
		indexBufferConfiguration(modelData, groupIndex),
		&indexBufferData(modelData, groupIndex).front()
		),
	indexCount_(modelData.drawGroups[groupIndex].indices.size()),
	primitiveTopology_(modelData.drawGroups[groupIndex].primitiveTopology),
	rasteriser_(graphicsRenderer, rasteriserConfiguration()),
	sampler_(graphicsRenderer, samplerConfiguration())
{
}

void DrawGroup::render(CommandBuffer& commandBuffer, PassContext PassContext) {
	auto drawCommand = std::make_unique<GeometryDrawCommand>(); // TODO: these need to be created in a separate class and buffered

	drawCommand->setRasteriser(&rasteriser_);
	drawCommand->addSampler(&sampler_, milk::graphics::ShaderType::PIXEL, 0);

	PassContext.material = material_.get();

	drawCommand->setInputLayout(&PassContext.pass->inputLayout());
	drawCommand->setVertexShader(&PassContext.pass->vertexShader().shaderData());
	PassContext.pass->vertexShader().bind(*drawCommand, PassContext);
	drawCommand->setPixelShader(&PassContext.pass->pixelShader().shaderData());
	PassContext.pass->pixelShader().bind(*drawCommand, PassContext);

	drawCommand->setVertexBuffer(&vertexBuffer_);
	drawCommand->setIndexBuffer(&indexBuffer_);
	drawCommand->setIndexCount(indexCount_);
	drawCommand->setPrimitiveTopology(primitiveTopology_);

	drawCommand->setRenderTarget(PassContext.backBuffer); // TODO
	drawCommand->setDepthStencil(PassContext.screenDepthStencil); // TODO
	drawCommand->setViewport(PassContext.viewport); // TODO

	commandBuffer.add(std::move(drawCommand));
}
