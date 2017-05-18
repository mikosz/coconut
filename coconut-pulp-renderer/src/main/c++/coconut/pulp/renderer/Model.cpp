#include "Model.hpp"

#include <algorithm>
#include <iterator>
#include <numeric>

#include "Actor.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;

namespace /* anonymous */ {

milk::graphics::VertexBuffer createVertexBuffer(
	milk::graphics::Renderer& graphicsRenderer,
	size_t totalVertices,
	const shader::Input& shaderInput,
	mesh::Mesh::Submeshes::iterator submeshIt,
	mesh::Mesh::Submeshes::iterator submeshEnd
	) {
	auto data = std::vector<milk::fs::Byte>(); // TODO: this should be a common type
	data.resize(shaderInput.vertexSize() * totalVertices);
	auto* bufferPtr = reinterpret_cast<void*>(data.data()); // TODO: when above todo is fixed, writeData and all should use it
	auto* endPtr = reinterpret_cast<void*>(data.data() + data.size());

	std::for_each(submeshIt, submeshEnd, [&shaderInput, &bufferPtr, endPtr](const auto& submesh) {
			auto properties = shader::Properties();

			std::for_each(
				submesh.vertices().begin(),
				submesh.vertices().end(),
				[&shaderInput, &bufferPtr, endPtr, &properties](const auto& vertex) {
						// TODO: should be nicer. Could set something of the sort of "default object"
						// in properties, and assume that vertex. prefixes each provided property
						// descriptor. Then bind only vertex and put the interface in a ReflectiveInterface.
						// Would then allow for polymorphic vertices, or vertices with differing properties.
						properties.rebind("position0", static_cast<const primitive::Primitive&>(vertex.position));
						properties.rebind("normal0", static_cast<const primitive::Primitive&>(vertex.normal));
						properties.rebind("texcoord0", static_cast<const primitive::Primitive&>(vertex.textureCoordinate));

						bufferPtr = shaderInput.writeVertex(bufferPtr, properties);

						assert(bufferPtr <= endPtr);
					}
				);
		});

	assert(bufferPtr == endPtr);
	
	auto configuration = milk::graphics::Buffer::Configuration(
		shaderInput.vertexSize() * totalVertices,
		shaderInput.vertexSize(),
		false,
		false,
		false,
		data.data()
		);

	return milk::graphics::VertexBuffer(graphicsRenderer, configuration);
}

size_t indexBufferStride(size_t maxIndex) {
	constexpr auto max2ByteIndex = std::numeric_limits<std::uint16_t>::max();
	return maxIndex <= max2ByteIndex ? 2 : 4;
}

std::vector<std::uint32_t> indexBufferData(
	mesh::Mesh::Submeshes::iterator submeshIt,
	mesh::Mesh::Submeshes::iterator submeshEnd,
	size_t totalIndices
	)
{
	const auto needsDegenerateIndices =
		(submeshIt->primitiveTopology() == milk::graphics::PrimitiveTopology::TRIANGLE_STRIP);
	const auto totalDegenerateIndices = std::distance(submeshIt, submeshEnd) - 1;

	auto data = std::vector<std::uint32_t>();
	data.resize(totalIndices + totalDegenerateIndices);
	auto* dataPtr = data.data();

	auto baseIndex = 0u;
	std::for_each(
		submeshIt,
		submeshEnd,
		[&dataPtr, &baseIndex, needsDegenerateIndices](const auto& submesh) {
			if (baseIndex != 0 && needsDegenerateIndices) {
				*dataPtr++ = milk::graphics::IndexBuffer::degenerateIndex<std::uint32_t>();
			}
			dataPtr = std::transform(
				submesh.indices().begin(),
				submesh.indices().end(),
				dataPtr,
				[baseIndex](auto index) {
					return static_cast<std::uint32_t>(index + baseIndex);
				}
				);
			baseIndex += static_cast<std::uint32_t>(submesh.vertices().size());
		});

	assert(dataPtr - data.data() == data.size());

	return data;
}

milk::graphics::IndexBuffer createIndexBuffer(
	milk::graphics::Renderer& graphicsRenderer,
	size_t indexCount,
	const shader::Input& shaderInput,
	mesh::Mesh::Submeshes::iterator submeshIt,
	mesh::Mesh::Submeshes::iterator submeshEnd
)
{
	const auto indices32 = indexBufferData(submeshIt, submeshEnd, indexCount);
	assert(!indices32.empty());

	const auto maxIndex = *std::max_element(indices32.begin(), indices32.end());

	auto indices16 = std::vector<std::uint16_t>();
	const void* indexData = indices32.data();

	const auto stride = indexBufferStride(maxIndex);

	if (stride == 2) {
		indices16.reserve(indices32.size());
		std::copy(indices32.begin(), indices32.end(), std::back_inserter(indices16));
		indexData = indices32.data();
	}

	const auto configuration = milk::graphics::Buffer::Configuration(
		stride * indexCount,
		stride,
		false,
		false,
		false,
		indexData
		);

	return milk::graphics::IndexBuffer(graphicsRenderer, configuration);
}

} // anonymous namespace

Model::Model(
	std::string id,
	Mesh mesh,
	milk::graphics::Renderer& graphicsRenderer,
	shader::PassFactory& passFactory,
	const milk::FilesystemContext& filesystemContext
	) :
	id_(std::move(id))
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

	if (shaderInput.vertexSize() > 0) {
		vertexBuffer = createVertexBuffer(graphicsRenderer, totalVertices, shaderInput, submeshIt, submeshEnd);
	}

	indexCount = std::accumulate(submeshIt, submeshEnd, zero, [](size_t v, const auto& submesh) {
			return v + submesh.indices().size();
		});

	if (indexCount > 0) {
		indexBuffer = createIndexBuffer(graphicsRenderer, indexCount, shaderInput, submeshIt, submeshEnd);
	}
}

void Model::DrawGroup::render(CommandBuffer& commandBuffer, PassContext passContext) {
	passContext.properties.bind("material", &material.shaderProperties());

	if (material.shaderPass().isInstanced() && passContext.actors->size() > 1) { // TODO this and next lines
		auto drawCommand = std::make_unique<DrawCommand>(); // TODO: these need to be created in a separate class and buffered

		drawCommand->setRenderState(&material.renderState());

		auto& pass = material.shaderPass();

		passContext.material = &material;

		drawCommand->setVertexBuffer(&vertexBuffer);
		drawCommand->setIndexBuffer(&indexBuffer);
		drawCommand->setIndexCount(indexCount);
		drawCommand->setPrimitiveTopology(primitiveTopology);
		
		const auto instanceBufferSize = passContext.actors->size() * pass.input().instanceSize();
		if (instanceBufferSize > instanceDataBuffer.size()) { // TODO: TEMP (update conditionally, not here possibly, but in Scene)
			auto buffer = std::vector<std::uint8_t>(instanceBufferSize);
			auto* outputPtr = reinterpret_cast<void*>(buffer.data());
			for (const auto& actor : *passContext.actors) {
				actor->bindShaderProperties(passContext.properties, "actor");
				outputPtr = pass.input().writeInstance(outputPtr, passContext.properties);
			}
			
			auto configuration = milk::graphics::Buffer::Configuration(
				instanceBufferSize,
				pass.input().instanceSize(),
				true,
				false,
				false,
				buffer.data()
				);
			instanceDataBuffer = milk::graphics::VertexBuffer(
				*passContext.graphicsRenderer,
				configuration
				);
		}

		pass.bind(*drawCommand, passContext.properties);

		drawCommand->setInstanceDataBuffer(&instanceDataBuffer);
		drawCommand->setInstanceCount(passContext.actors->size());

		if (passContext.backBuffer) {
			drawCommand->setRenderTarget(*passContext.backBuffer); // TODO
		}
		if (passContext.screenDepthStencil) {
			drawCommand->setDepthStencil(*passContext.screenDepthStencil); // TODO
		}
		drawCommand->setViewport(passContext.viewport); // TODO

		commandBuffer.add(std::move(drawCommand));
	} else {
		for (const auto& actor : *passContext.actors) {
			actor->bindShaderProperties(passContext.properties, "actor");

			auto drawCommand = std::make_unique<DrawCommand>(); // TODO: these need to be created in a separate class and buffered
				// TODO: duplicated code above
			drawCommand->setRenderState(&material.renderState());

			auto& pass = material.shaderPass();

			passContext.material = &material;

			drawCommand->setVertexBuffer(&vertexBuffer);
			drawCommand->setIndexBuffer(&indexBuffer);
			drawCommand->setIndexCount(indexCount);
			drawCommand->setPrimitiveTopology(primitiveTopology);

			pass.bind(*drawCommand, passContext.properties);

			if (passContext.backBuffer) {
				drawCommand->setRenderTarget(*passContext.backBuffer); // TODO
			}
			if (passContext.screenDepthStencil) {
				drawCommand->setDepthStencil(*passContext.screenDepthStencil); // TODO
			}
			drawCommand->setViewport(passContext.viewport); // TODO

			drawCommand->setInstanceCount(1);

			commandBuffer.add(std::move(drawCommand));
		}
	}
}
