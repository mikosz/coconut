#include "Model.hpp"

#include <algorithm>
#include <iterator>
#include <numeric>

#include "coconut/milk/graphics/Texture2d.hpp"
#include "coconut/milk/graphics/Sampler.hpp"
#include "coconut/milk/graphics/ImageLoader.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;

namespace /* anonymous */ {

milk::graphics::Buffer::Configuration vertexBufferConfiguration(
	size_t vertexCount,
	size_t vertexSize
	) {
	return milk::graphics::Buffer::Configuration(
		vertexSize * vertexCount,
		vertexSize,
		false,
		false,
		false
		);
}

std::vector<milk::fs::Byte> vertexBufferData(
	const shader::Input& shaderInput,
	mesh::Mesh::Submeshes::iterator submeshIt,
	mesh::Mesh::Submeshes::iterator submeshEnd,
	size_t totalVertices
	)
{
	auto data = std::vector<milk::fs::Byte>(); // TODO: this should be a common type
	data.resize(shaderInput.vertexSize() * totalVertices);
	auto* bufferPtr = data.data();

	std::for_each(submeshIt, submeshEnd, [&shaderInput, &bufferPtr](const auto& submesh) {
			std::for_each(
				submesh.vertices().begin(),
				submesh.vertices().end(),
				[&shaderInput, &bufferPtr](const auto& vertex) {
					shaderInput.writeVertex(bufferPtr, &vertex);
					bufferPtr += shaderInput.vertexSize();
				}
				);
		});

	return data;
}

size_t indexBufferStride(size_t totalVertices) {
	const auto max2ByteIndex = std::numeric_limits<std::uint16_t>::max();
	return totalVertices <= max2ByteIndex ? 2 : 4;
}

milk::graphics::Buffer::Configuration indexBufferConfiguration(size_t totalIndices, size_t totalVertices) {
	const auto stride = indexBufferStride(totalVertices);
	auto configuration = milk::graphics::Buffer::Configuration(
		stride * totalIndices,
		stride,
		false,
		false,
		false
		);

	return configuration;
}

template <class IndexType>
std::vector<std::uint8_t> indexBufferData(
	mesh::Mesh::Submeshes::iterator submeshIt,
	mesh::Mesh::Submeshes::iterator submeshEnd,
	size_t totalIndices
	)
{
	const auto needsDegenerateIndices =
		(submeshIt->primitiveTopology() == milk::graphics::PrimitiveTopology::TRIANGLE_STRIP);
	const auto totalDegenerateIndices = std::distance(submeshIt, submeshEnd) - 1;

	auto data = std::vector<std::uint8_t>();
	data.reserve(sizeof(IndexType) * (totalIndices + totalDegenerateIndices));

	auto* dataPtr = reinterpret_cast<IndexType*>(data.data());

	auto baseIndex = static_cast<IndexType>(0);
	std::for_each(
		submeshIt,
		submeshEnd,
		[&dataPtr, &baseIndex, needsDegenerateIndices](const auto& submesh) {
			if (baseIndex != 0 && needsDegenerateIndices) {
				*dataPtr++ = milk::graphics::IndexBuffer::degenerateIndex<IndexType>();
			}
			std::transform(
				submesh.indices().begin(),
				submesh.indices().end(),
				dataPtr,
				[baseIndex](auto index) {
					return static_cast<IndexType>(index + baseIndex);
				}
				);
			baseIndex += static_cast<IndexType>(submesh.vertices().size());
		});

	assert(dataPtr - reinterpret_cast<IndexType*>(data.data()) == data.size());

	return data;
}

} // anonymous namespace

Model::Model(
	Mesh mesh,
	milk::graphics::Renderer& graphicsRenderer,
	shader::PassFactory& passFactory,
	const milk::FilesystemContext& filesystemContext
	)
{
	assert(!mesh.submeshes().empty());

	auto submeshMaterialComparator = [](const auto& lhs, const auto& rhs) {
			return lhs.materialId() < rhs.materialId();
		};

	auto& submeshes = mesh.submeshes();
	std::sort(submeshes.begin(), submeshes.end(), submeshMaterialComparator);

	auto it = submeshes.begin();
	while (it != submeshes.end()) {
		auto end = mesh::Mesh::Submeshes::iterator();
		std::tie(it, end) = std::equal_range(it, submeshes.end(), *it, submeshMaterialComparator);
		if (it != submeshes.end()) {
			drawGroups_.emplace_back(
				graphicsRenderer,
				passFactory,
				filesystemContext,
				it,
				end,
				mesh.materials()[it->materialId()]
				);
			it = end;
		}
	}
}

void Model::render(CommandBuffer& commandBuffer, PassContext passContext) {
	passContext.model = this;

	for (auto& drawGroup : drawGroups_) {
		drawGroup.render(commandBuffer, passContext);
	}
}

Model::DrawGroup::DrawGroup(
	milk::graphics::Renderer& graphicsRenderer,
	shader::PassFactory& passFactory,
	const milk::fs::FilesystemContext& filesystemContext,
	mesh::Mesh::Submeshes::iterator submeshIt,
	mesh::Mesh::Submeshes::iterator submeshEnd,
	const mesh::MaterialConfiguration& materialConfiguration
	) :
	primitiveTopology(submeshIt->primitiveTopology()),
	material(graphicsRenderer, passFactory, filesystemContext, materialConfiguration)
{
	const auto zero = static_cast<size_t>(0);

	const auto totalVertices = std::accumulate(submeshIt, submeshEnd, zero, [](size_t v, const auto& submesh) {
			return v + submesh.vertices().size();
		});
	const auto& shaderInput = material.shaderPass().input();

	vertexBuffer = milk::graphics::VertexBuffer(
		graphicsRenderer,
		vertexBufferConfiguration(totalVertices, shaderInput.vertexSize()),
		vertexBufferData(shaderInput, submeshIt, submeshEnd, totalVertices).data()
		);

	const auto totalIndices = std::accumulate(submeshIt, submeshEnd, zero, [](size_t v, const auto& submesh) {
			return v + submesh.indices().size();
		});

	const auto indexSize = indexBufferStride(totalVertices);
	if (indexSize == 2) {
		indexBuffer = milk::graphics::IndexBuffer(
			graphicsRenderer,
			indexBufferConfiguration(totalIndices, totalVertices),
			indexBufferData<std::uint16_t>(submeshIt, submeshEnd, totalIndices).data()
			);
	} else if (indexSize == 4) {
		indexBuffer = milk::graphics::IndexBuffer(
			graphicsRenderer,
			indexBufferConfiguration(totalIndices, totalVertices),
			indexBufferData<std::uint32_t>(submeshIt, submeshEnd, totalIndices).data()
			);
	} else {
		assert(!("Unexpected index size: " + std::to_string(indexSize)).c_str());
	}
}

void Model::DrawGroup::render(CommandBuffer& commandBuffer, PassContext passContext) {
	auto drawCommand = std::make_unique<DrawCommand>(); // TODO: these need to be created in a separate class and buffered

	drawCommand->setRenderState(&material.renderState());

	auto& pass = material.shaderPass();

	drawCommand->setInputLayout(&pass.input().layout());
	drawCommand->setVertexShader(&pass.vertexShader().shaderData());
	pass.vertexShader().bind(*drawCommand, passContext);
	drawCommand->setPixelShader(&pass.pixelShader().shaderData());
	pass.pixelShader().bind(*drawCommand, passContext);

	drawCommand->setVertexBuffer(vertexBuffer.get_ptr());
	if (instanceDataBuffer) {
		drawCommand->setInstanceDataBuffer(instanceDataBuffer.get_ptr());
	}
	if (indexBuffer) {
		drawCommand->setIndexBuffer(indexBuffer.get_ptr());
	}
	drawCommand->setIndexCount(indexCount);
	drawCommand->setPrimitiveTopology(primitiveTopology);

	drawCommand->setRenderTarget(passContext.backBuffer); // TODO
	drawCommand->setDepthStencil(passContext.screenDepthStencil); // TODO
	drawCommand->setViewport(passContext.viewport); // TODO

	drawCommand->setInstanceCount(instanceCount);

	commandBuffer.add(std::move(drawCommand));
}
