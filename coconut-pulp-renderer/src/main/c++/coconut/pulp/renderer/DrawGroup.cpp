#include "DrawGroup.hpp"

#include <algorithm>
#include <cstdint>
#include <limits>

#include "shader/Pass.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;

namespace /* anonymous */ {

milk::graphics::Buffer::Configuration vertexBufferConfiguration(const DrawGroup::Data& drawGroupData) {
	milk::graphics::Buffer::Configuration configuration;

	configuration.allowCPURead = false;
	configuration.allowGPUWrite = false;
	configuration.allowModifications = false;
	configuration.purpose =  milk::graphics::Buffer::CreationPurpose::VERTEX_BUFFER;
	configuration.stride = drawGroupData.inputLayout->vertexSize();
	configuration.size = configuration.stride * drawGroupData.vertices.size();

	return configuration;
}

milk::graphics::Buffer::Configuration indexBufferConfiguration(const DrawGroup::Data& drawGroupData) {
	milk::graphics::Buffer::Configuration configuration;

	configuration.allowCPURead = false;
	configuration.allowGPUWrite = false;
	configuration.allowModifications = false;
	configuration.purpose =  milk::graphics::Buffer::CreationPurpose::INDEX_BUFFER;
	// TODO: index size should be 2 or 4 depending on max index, this is hardcoded to 4
	configuration.stride = 4;
	configuration.size = configuration.stride * drawGroupData.indices.size();

	return configuration;
}

std::vector<std::uint8_t> vertexBufferData(const DrawGroup::Data& drawGroupData) {
	std::vector<std::uint8_t> data;
	const size_t vertexSize = drawGroupData.inputLayout->vertexSize();
	data.resize(vertexSize * drawGroupData.vertices.size());

	std::uint8_t* target = &data.front();
	for (auto vertex : drawGroupData.vertices) {
		drawGroupData.inputLayout->makeVertex(*vertex, target);
		target += vertexSize;
	}

	return data;
}

std::vector<std::uint8_t> indexBufferData(const DrawGroup::Data& drawGroupData) {
	std::vector<std::uint8_t> data;
	data.resize(4 * drawGroupData.indices.size());

	// TODO: index size should be 2 or 4 depending on max index, this is hardcoded to 4
	std::uint32_t* target = reinterpret_cast<std::uint32_t*>(&data.front());
	for (auto index : drawGroupData.indices) {
		*target = static_cast<std::uint32_t>(index);
		++target;
	}

	return data;
}

} /* anonymous namespace */

DrawGroup::DrawGroup(milk::graphics::Device& graphicsDevice, const Data& data) :
	material_(data.material),
	vertexBuffer_(graphicsDevice, vertexBufferConfiguration(data), &vertexBufferData(data).front()),
	indexBuffer_(graphicsDevice, indexBufferConfiguration(data), &indexBufferData(data).front()),
	indexCount_(data.indices.size()),
	primitiveTopology_(data.primitiveTopology)
{
}

void DrawGroup::render(milk::graphics::Device& graphicsDevice, RenderingContext renderingContext) {
	vertexBuffer_.bind(graphicsDevice, milk::graphics::ShaderType::VERTEX, 0);
	indexBuffer_.bind(graphicsDevice, milk::graphics::ShaderType::PIXEL, 0);

	renderingContext.material = material_.get();

	renderingContext.pass->bind(graphicsDevice, renderingContext);
	renderingContext.pass->bind(graphicsDevice, renderingContext);

	graphicsDevice.draw(0, indexCount_, primitiveTopology_);
}
