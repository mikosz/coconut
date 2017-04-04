#include "Grass.hpp"

#include <coconut-tools/utils/Range.hpp>

#include "coconut/pulp/renderer/CommandBuffer.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::world;
using namespace coconut::pulp::world::foliage;

using namespace std::string_literals;

namespace /* anonymous */ {

Mesh grassMesh() {
	auto submeshes = Mesh::Submeshes();
	auto materials = Mesh::MaterialConfigurations();

	const auto bladeMaterialId = "blade"s;

	const auto bottomLeft = primitive::Position(-0.03f, 0.0f, 0.0f);
	const auto bottomRight = primitive::Position(0.03f, 0.0f, 0.0f);
	const auto topLeft = primitive::Position(-0.03f, 0.5f, 0.0f);
	const auto topRight = primitive::Position(0.03f, 0.5f, 0.0f);

	const auto texBottomLeft = primitive::TextureCoordinate(0.0f, 0.0f);
	const auto texBottomRight = primitive::TextureCoordinate(1.0f, 0.0f);
	const auto texTopLeft = primitive::TextureCoordinate(0.0f, 1.0f);
	const auto texTopRight = primitive::TextureCoordinate(1.0f, 1.0f);

	// blades
	for (auto x: coconut_tools::range(-0.5f, 0.5f, 0.5f)) {
		for (auto z : coconut_tools::range(-0.5f, 0.5f, 0.5f)) {
			auto vertices = Submesh::Vertices();
			auto indices = Submesh::Indices();

			auto vertex = Submesh::Vertex();
			vertex.normal = primitive::Vector(0.0f, 0.0f, -1.0f);

			const auto offset = primitive::Vector(x, 0.0f, z);

			vertex.position = bottomRight + offset;
			vertex.textureCoordinate = texBottomRight;
			indices.emplace_back(vertices.size());
			vertices.emplace_back(vertex);

			vertex.position = bottomLeft + offset;
			vertex.textureCoordinate = texBottomLeft;
			indices.emplace_back(vertices.size());
			vertices.emplace_back(vertex);

			vertex.position = topRight + offset;
			vertex.textureCoordinate = texTopRight;
			indices.emplace_back(vertices.size());
			vertices.emplace_back(vertex);

			vertex.position = topLeft + offset;
			vertex.textureCoordinate = texTopLeft;
			indices.emplace_back(vertices.size());
			vertices.emplace_back(vertex);

			submeshes.emplace_back(
				std::move(vertices),
				std::move(indices),
				std::move(bladeMaterialId),
				milk::graphics::PrimitiveTopology::TRIANGLE_STRIP
				);
		}
	}
	
	auto material = mesh::MaterialConfiguration();

	material.properties()[mesh::MaterialConfiguration::AMBIENT_COLOUR_PROPERTY] = primitive::Colour(0.0f, 0.01f, 0.0f);
	material.properties()[mesh::MaterialConfiguration::SPECULAR_COLOUR_PROPERTY] = primitive::Colour(0.0f, 0.03f, 0.0f);
	material.properties()[mesh::MaterialConfiguration::SPECULAR_EXPONENT_PROPERTY] = primitive::Scalar(0.1f);
	material.properties()[mesh::MaterialConfiguration::DIFFUSE_COLOUR_PROPERTY] = primitive::Colour(0.0f, 0.7f, 0.0f);

	material.renderStateConfiguration().cullMode = milk::graphics::RenderState::CullMode::NONE;
	material.renderStateConfiguration().fillMode = milk::graphics::RenderState::FillMode::SOLID;
	material.renderStateConfiguration().frontCounterClockwise = false;

	material.passType() = mesh::MaterialConfiguration::PassType::OPAQUE;

	material.shaderName() = "grass"s;

	auto samplerConfiguration = milk::graphics::Sampler::Configuration();
	samplerConfiguration.addressModeU = milk::graphics::Sampler::AddressMode::WRAP;
	samplerConfiguration.addressModeV = milk::graphics::Sampler::AddressMode::WRAP;
	samplerConfiguration.addressModeW = milk::graphics::Sampler::AddressMode::WRAP;
	samplerConfiguration.filter = milk::graphics::Sampler::Filter::MIN_MAG_MIP_LINEAR; // TODO
	material.addTexture(mesh::MaterialConfiguration::DIFFUSE_MAP_TEXTURE, "/data/textures/grass.png"s, std::move(samplerConfiguration));

	materials.emplace("blade"s, std::move(material));

	// patches
	//for (auto x : coconut_tools::range(-25.0f, 25.0f, 1.0f)) {
	//	for (auto z : coconut_tools::range(-25.0f, 25.0f, 1.0f)) {
	//		drawGroup.instances.emplace_back();
	//		drawGroup.instances.back().patchPosition = pulp::math::Vec4(x, 0.0f, z, 1.0f);
	//	}
	//}

	return Mesh(std::move(submeshes), std::move(materials));
}

} // anonymous namespace

Grass::Grass(
	milk::graphics::Renderer& graphicsRenderer,
	renderer::shader::PassFactory& passFactory,
	milk::FilesystemContext& filesystemContext
	) :
	Actor(std::make_shared<renderer::Model>(
		grassMesh(),
		graphicsRenderer,
		passFactory,
		filesystemContext
		))
{
}

void Grass::update(std::chrono::milliseconds dt) {

}

void Grass::render(renderer::CommandBuffer& commandBuffer, renderer::PassContext PassContext) {
	renderer::Actor::render(commandBuffer, PassContext);
}
