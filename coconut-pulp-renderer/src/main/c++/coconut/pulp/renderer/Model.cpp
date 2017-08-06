#include "Model.hpp"

#include <algorithm>
#include <iterator>
#include <numeric>

#include "Actor.hpp"

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

	return data;
}

size_t indexBufferStride(size_t totalVertices) {
	const auto max2ByteIndex = std::numeric_limits<std::uint16_t>::max();
	return totalVertices <= max2ByteIndex ? 2 : 4;
}

milk::graphics::Buffer::Configuration indexBufferConfiguration(size_t totalIndices, size_t stride) {
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
	data.resize(sizeof(IndexType) * (totalIndices + totalDegenerateIndices));

	auto* dataPtr = reinterpret_cast<IndexType*>(data.data());

	auto baseIndex = static_cast<IndexType>(0);
	std::for_each(
		submeshIt,
		submeshEnd,
		[&dataPtr, &baseIndex, needsDegenerateIndices](const auto& submesh) {
			if (baseIndex != 0 && needsDegenerateIndices) {
				*dataPtr++ = milk::graphics::IndexBuffer::degenerateIndex<IndexType>();
			}
			dataPtr = std::transform(
				submesh.indices().begin(),
				submesh.indices().end(),
				dataPtr,
				[baseIndex](auto index) {
					return static_cast<IndexType>(index + baseIndex);
				}
				);
			baseIndex += static_cast<IndexType>(submesh.vertices().size());
		});

	assert(
		static_cast<size_t>(dataPtr - reinterpret_cast<IndexType*>(data.data())) ==
		data.size() / sizeof(IndexType)
		);

	return data;
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
			auto drawGroup = DrawGroup(
				graphicsRenderer,
				passFactory,
				filesystemContext,
				it,
				end,
				mesh.materials()[it->materialId()]
				);
			drawGroups_.emplace_back(std::move(drawGroup));
			// TODO: find out a way to use this without VS getting lost in case of exceptions
			//drawGroups_.emplace_back(
			//	graphicsRenderer,
			//	passFactory,
			//	filesystemContext,
			//	it,
			//	end,
			//	mesh.materials()[it->materialId()]
			//	);
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
		vertexBuffer = milk::graphics::VertexBuffer(
			graphicsRenderer,
			vertexBufferConfiguration(totalVertices, shaderInput.vertexSize()),
			vertexBufferData(shaderInput, submeshIt, submeshEnd, totalVertices).data()
			);
	}

	indexCount = std::accumulate(submeshIt, submeshEnd, zero, [](size_t v, const auto& submesh) {
			return v + submesh.indices().size();
		});

	// TODO: should be max index, not indexCount
	const auto indexSize = indexBufferStride((totalVertices > 0) ? totalVertices : indexCount);
	if (indexSize == 2) {
		indexBuffer = milk::graphics::IndexBuffer(
			graphicsRenderer,
			indexBufferConfiguration(indexCount, indexSize),
			indexBufferData<std::uint16_t>(submeshIt, submeshEnd, indexCount).data()
			);
	} else if (indexSize == 4) {
		indexBuffer = milk::graphics::IndexBuffer(
			graphicsRenderer,
			indexBufferConfiguration(indexCount, indexSize),
			indexBufferData<std::uint32_t>(submeshIt, submeshEnd, indexCount).data()
			);
	} else {
		assert(!("Unexpected index size: " + std::to_string(indexSize)).c_str());
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
		if (instanceBufferSize > instanceDataBuffer.configuration().size) { // TODO: TEMP (update conditionally, not here possibly, but in Scene)
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
				false
				);
			instanceDataBuffer = milk::graphics::VertexBuffer(
				*passContext.graphicsRenderer,
				configuration,
				buffer.data()
				);
		}

		pass.bind(*drawCommand, passContext.properties);

		drawCommand->setInstanceDataBuffer(&instanceDataBuffer);
		drawCommand->setInstanceCount(passContext.actors->size());

		drawCommand->setRenderTarget(passContext.backBuffer); // TODO
		drawCommand->setDepthStencil(passContext.screenDepthStencil); // TODO
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

			drawCommand->setRenderTarget(passContext.backBuffer); // TODO
			drawCommand->setDepthStencil(passContext.screenDepthStencil); // TODO
			drawCommand->setViewport(passContext.viewport); // TODO

			drawCommand->setInstanceCount(1);

			commandBuffer.add(std::move(drawCommand));
		}
	}
}
