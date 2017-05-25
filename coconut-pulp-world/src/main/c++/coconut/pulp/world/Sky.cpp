#include "Sky.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::world;

using namespace std::string_literals;

namespace /* anonymous */ {

renderer::ActorSharedPtr createSkydomeActor() {
	return std::make_unique<renderer::Actor>("skydome");
}

renderer::ModelSharedPtr createSkydomeModel(
	milk::graphics::Renderer& graphicsRenderer,
	renderer::shader::PassFactory& passFactory,
	const milk::FilesystemContext& fs
	)
{
	auto submeshes = Mesh::Submeshes();

	// TODO: hardcoded
	const auto sliceCount = 30u;
	const auto stackCount = 30u;
	const auto radius = 500.0f;

	// TODO: create a geometry generator?
	auto vertices = Submesh::Vertices();

	auto topVertex = Submesh::Vertex();
	topVertex.position = Position(0.0f, radius, 0.0f);
	vertices.emplace_back(topVertex);

	const auto phiStep = PI / stackCount;
	const auto thetaStep = 2.0f * PI / sliceCount;

	for (const auto stackIndex : coconut_tools::range(1u, stackCount)) {
		const auto phi = stackIndex * phiStep;

		for (const auto sliceIndex : coconut_tools::range(0u, sliceCount + 1)) {
			const auto theta = sliceIndex * thetaStep;

			auto vertex = Submesh::Vertex();
			vertex.position = Vec3(
				radius * std::sin(phi) * std::cos(theta),
				radius * std::cos(phi),
				radius * std::sin(phi) * std::sin(theta)
				);
			vertices.emplace_back(vertex);
		}
	}

	auto bottomVertex = Submesh::Vertex();
	bottomVertex.position = Position(0.0f, -radius, 0.0f);
	vertices.emplace_back(bottomVertex);

	auto indices = Submesh::Indices();

	for (const auto index : coconut_tools::range(1u, sliceCount + 1)) {
		indices.emplace_back(0);
		indices.emplace_back(index + 1);
		indices.emplace_back(index);
	}

	for (const auto stackIndex : coconut_tools::range(0u, stackCount - 2)) {
		for (const auto sliceIndex : coconut_tools::range(0u, sliceCount)) {
			indices.emplace_back(1 + stackIndex * (sliceCount + 1) + sliceIndex);
			indices.emplace_back(1 + stackIndex * (sliceCount + 1) + sliceIndex + 1);
			indices.emplace_back(1 + (stackIndex + 1) * (sliceCount + 1) + sliceIndex);

			indices.emplace_back(1 + (stackIndex + 1) * (sliceCount + 1) + sliceIndex);
			indices.emplace_back(1 + stackIndex * (sliceCount + 1) + sliceIndex + 1);
			indices.emplace_back(1 + (stackIndex + 1) * (sliceCount + 1) + sliceIndex + 1);
		}
	}

	const auto bottomIndex = vertices.size() - 1;

	for (const auto index : coconut_tools::range(0u, sliceCount)) {
		indices.emplace_back(bottomIndex);
		indices.emplace_back(bottomIndex - sliceCount - 1 + index);
		indices.emplace_back(bottomIndex - sliceCount - 1 + index + 1);
	}

	submeshes.emplace_back(
		std::move(vertices),
		std::move(indices),
		"skydome"s,
		milk::graphics::PrimitiveTopology::TRIANGLE_LIST
		);

	auto materialConfiguration = MaterialConfiguration();

	auto renderStateConfiguration = milk::graphics::RenderState::Configuration();
	renderStateConfiguration.cullMode = milk::graphics::RenderState::CullMode::BACK;
	renderStateConfiguration.fillMode = milk::graphics::RenderState::FillMode::SOLID;
	renderStateConfiguration.frontCounterClockwise = true; // false; // TODO: ???

	auto skydomeSamplerConfiguration = milk::graphics::Sampler::Configuration();
	skydomeSamplerConfiguration.addressModeU = milk::graphics::Sampler::AddressMode::WRAP;
	skydomeSamplerConfiguration.addressModeV = milk::graphics::Sampler::AddressMode::WRAP;
	skydomeSamplerConfiguration.addressModeW = milk::graphics::Sampler::AddressMode::WRAP;
	skydomeSamplerConfiguration.filter = milk::graphics::Sampler::Filter::MIN_MAG_MIP_LINEAR;

	materialConfiguration.passType() = MaterialConfiguration::PassType::OPAQUE;
	materialConfiguration.shaderName() = "sky"s;
	materialConfiguration.renderStateConfiguration() = renderStateConfiguration;
	materialConfiguration.addTexture(
		"skydome",
		fs.makeAbsolute("data/terrain/grass-skydome.dds"),
		skydomeSamplerConfiguration
		);

	auto model = std::make_unique<renderer::Model>(
		"skydome"s,
		Mesh(std::move(submeshes), { { "skydome"s, std::move(materialConfiguration) } }),
		graphicsRenderer,
		passFactory,
		fs
		);

	return model;
}

} // anonymous namespace

Sky::Sky(
	milk::graphics::Renderer& graphicsRenderer,
	renderer::Scene& scene,
	renderer::shader::PassFactory& passFactory, // TODO: inconvenient
	const milk::FilesystemContext& fs
	)
{
	// TODO: have to be able to tell that the skydome is to be rendered at the end
	scene.add(createSkydomeActor(), createSkydomeModel(graphicsRenderer, passFactory, fs));
}
