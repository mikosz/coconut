#include "Grass.hpp"

#include <coconut-tools/utils/Range.hpp>

#include "coconut/pulp/renderer/CommandBuffer.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::world;
using namespace coconut::pulp::world::foliage;

namespace /* anonymous */ {

model::Data generateInstancedPatchData() {
	auto data = model::Data();

	data.rasteriserConfiguration.cullMode = milk::graphics::Rasteriser::CullMode::BACK;
	data.rasteriserConfiguration.fillMode = milk::graphics::Rasteriser::FillMode::SOLID;
	data.rasteriserConfiguration.frontCounterClockwise = false;

	data.normals.emplace_back(0.0f, 0.0f, -1.0f);

	data.textureCoordinates.emplace_back(0.0f, 0.0f);
	data.textureCoordinates.emplace_back(1.0f, 0.0f);
	data.textureCoordinates.emplace_back(0.0f, 1.0f);
	data.textureCoordinates.emplace_back(1.0f, 1.0f);

	data.phongMaterials.emplace_back();
	data.phongMaterials.back().ambientColour = milk::math::Vector4d(0.0f, 1.0f, 0.0f, 1.0f);
	data.phongMaterials.back().diffuseColour = milk::math::Vector4d(0.0f, 1.0f, 0.0f, 1.0f);
	data.phongMaterials.back().specularColour = milk::math::Vector4d(0.0f, 0.1f, 0.0f, 1.0f);
	data.phongMaterials.back().diffuseMap = "/data/textures/grass.png";
	data.phongMaterials.back().diffuseMapSamplerConfiguration.addressModeU = milk::graphics::Sampler::AddressMode::WRAP;
	data.phongMaterials.back().diffuseMapSamplerConfiguration.addressModeV = milk::graphics::Sampler::AddressMode::WRAP;
	data.phongMaterials.back().diffuseMapSamplerConfiguration.addressModeW = milk::graphics::Sampler::AddressMode::WRAP;
	data.phongMaterials.back().diffuseMapSamplerConfiguration.filter = milk::graphics::Sampler::Filter::MIN_MAG_MIP_LINEAR;
	data.phongMaterials.back().name = "grass::blade";

	data.drawGroups.emplace_back();
	auto& drawGroup = data.drawGroups.back();

	drawGroup.primitiveTopology = milk::graphics::PrimitiveTopology::TRIANGLE_LIST;
	drawGroup.materialId = "grass::blade";

	// blades
	for (auto x: coconut_tools::range(-0.5f, 0.5f, 0.5f)) {
		for (auto z : coconut_tools::range(-0.5f, 0.5f, 0.5f)) {
			auto offset = milk::math::Vector3d(x, 0.0f, z);

			auto bottomLeft = milk::math::Vector3d(-0.03f, 0.0f, 0.0f);
			auto bottomRight = milk::math::Vector3d(0.03f, 0.0f, 0.0f);
			auto topLeft = milk::math::Vector3d(-0.03f, 0.5f, 0.0f);
			auto topRight = milk::math::Vector3d(0.03f, 0.5f, 0.0f);

			drawGroup.vertices.emplace_back();
			drawGroup.vertices.back().positionIndex = data.positions.size();
			drawGroup.vertices.back().normalIndex = 0;
			drawGroup.vertices.back().textureCoordinateIndex = 1;
			drawGroup.indices.emplace_back(data.positions.size());
			data.positions.emplace_back(bottomRight + offset);

			drawGroup.vertices.emplace_back();
			drawGroup.vertices.back().positionIndex = data.positions.size();
			drawGroup.vertices.back().normalIndex = 0;
			drawGroup.vertices.back().textureCoordinateIndex = 0;
			drawGroup.indices.emplace_back(data.positions.size());
			data.positions.emplace_back(bottomLeft + offset);

			drawGroup.vertices.emplace_back();
			drawGroup.vertices.back().positionIndex = data.positions.size();
			drawGroup.vertices.back().normalIndex = 0;
			drawGroup.vertices.back().textureCoordinateIndex = 2;
			drawGroup.indices.emplace_back(data.positions.size());
			data.positions.emplace_back(topLeft + offset);

			// ---

			drawGroup.vertices.emplace_back();
			drawGroup.vertices.back().positionIndex = data.positions.size();
			drawGroup.vertices.back().normalIndex = 0;
			drawGroup.vertices.back().textureCoordinateIndex = 1;
			drawGroup.indices.emplace_back(data.positions.size());
			data.positions.emplace_back(bottomRight + offset);

			drawGroup.vertices.emplace_back();
			drawGroup.vertices.back().positionIndex = data.positions.size();
			drawGroup.vertices.back().normalIndex = 0;
			drawGroup.vertices.back().textureCoordinateIndex = 2;
			drawGroup.indices.emplace_back(data.positions.size());
			data.positions.emplace_back(topLeft + offset);

			drawGroup.vertices.emplace_back();
			drawGroup.vertices.back().positionIndex = data.positions.size();
			drawGroup.vertices.back().normalIndex = 0;
			drawGroup.vertices.back().textureCoordinateIndex = 3;
			drawGroup.indices.emplace_back(data.positions.size());
			data.positions.emplace_back(topRight + offset);
		}
	}

	// patches
	for (auto x : coconut_tools::range(-25.0f, 25.0f, 1.0f)) {
		for (auto z : coconut_tools::range(-25.0f, 25.0f, 1.0f)) {
			drawGroup.instances.emplace_back();
			drawGroup.instances.back().patchPosition = milk::math::Vector4d(x, 0.0f, z, 1.0f);
		}
	}

	return data;
}

} // anonymous namespace

Grass::Grass(
	milk::graphics::Renderer& graphicsRenderer,
	const renderer::shader::Input& input,
	renderer::MaterialManager& materialManager,
	milk::FilesystemContext& filesystemContext
	) :
	Actor(std::make_shared<renderer::Model>(
		generateInstancedPatchData(),
		graphicsRenderer,
		input,
		materialManager,
		filesystemContext
		))
{
}

void Grass::update(std::chrono::milliseconds dt) {

}

void Grass::render(renderer::CommandBuffer& commandBuffer, renderer::PassContext PassContext) {
	renderer::Actor::render(commandBuffer, PassContext);
}
