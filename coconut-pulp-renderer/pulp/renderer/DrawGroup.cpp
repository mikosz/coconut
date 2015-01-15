#include "DrawGroup.hpp"

#include <cstdint>

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;

namespace {

milk::graphics::Buffer::Configuration vertexBufferConfiguration(const DrawGroup::Data& drawGroupData) {
	milk::graphics::Buffer::Configuration configuration;

	configuration.allowCPURead = false;
	configuration.allowGPUWrite = false;
	configuration.allowModifications = false;
	configuration.purpose =  milk::graphics::Buffer::CreationPurpose::VERTEX_BUFFER;
	configuration.stride = drawGroupData.material.inputLayout().vertexSize();
	configuration.size = configuration.stride * drawGroupData.vertexCount;
}

milk::graphics::Buffer::Configuration indexBufferConfiguration(const DrawGroup::Data& drawGroupData) {
	milk::graphics::Buffer::Configuration configuration;

	configuration.allowCPURead = false;
	configuration.allowGPUWrite = false;
	configuration.allowModifications = false;
	configuration.purpose =  milk::graphics::Buffer::CreationPurpose::INDEX_BUFFER;
	configuration.stride = drawGroupData.material.indexSize();
	configuration.size = configuration.stride * drawGroupData.indexCount;
}

// TODO: make sure this is move-constructed
std::vector<std::uint8_t> vertexBufferData(const DrawGroup::Data& drawGroupData) {
	std::vector<std::uint8_t> data;
	const size_t vertexSize = drawGroupData.material.inputLayout().vertexSize();
	data.resize(vertexSize * drawGroupData.vertexCount);

	std::uint8_t* target = &data.front();
	for (auto vertex : drawGroupData.vertices) {
		drawGroupData.material.inputLayout().makeVertex(vertex, target);
		target += vertexSize;
	}

	return data;
}

// TODO: make sure this is move-constructed
std::vector<std::uint8_t> indexBufferData(const DrawGroup::Data& drawGroupData) {
	std::vector<std::uint8_t> data;
	const size_t indexSize = drawGroupData.material.inputLayout().indexSize();
	data.resize(indexSize * drawGroupData.indexCount);

	if (indexSize != 2 && indexSize != 4) {
		throw std::runtime_error("Unexpected index size");
	}

	std::uint8_t* target = &data.front();
	for (auto index : drawGroupData.indices) {
		if (indexSize == 2) {
			*target = static_cast<std::uint16_t>(index);
		} else {
			*target = static_cast<std::uint32_t>(index);
		}

		target += indexSize;
	}

	return data;
}

} /* anonymous namespace */

DrawGroup::DrawGroup(milk::graphics::Device& graphicsDevice, const Data& data) :
	material_(data.material),
	vertexBuffer_(graphicsDevice, vertexBufferConfiguration(data), &vertexBufferData(data).front()),
	indexBuffer_(graphicsDevice, indexBufferConfiguration(data), &indexBufferData(data).front()),
	indexCount_(data.indexCount),
	primitiveTopology_(data.primitiveTopology)
{
}

void DrawGroup::render(milk::graphics::Device& graphicsDevice, RenderingContext renderingContext) {
	vertexBuffer_.bind(graphicsDevice, milk::graphics::Buffer::ShaderType::VERTEX, 0);
	indexBuffer_.bind(graphicsDevice, milk::graphics::Buffer::ShaderType::PIXEL, 0);

	material_->bind(graphicsDevice, renderingContext);

	graphicsDevice.draw(0, indexCount_, primitiveTopology_);
}
