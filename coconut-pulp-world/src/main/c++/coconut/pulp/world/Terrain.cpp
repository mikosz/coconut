#include "Terrain.hpp"

#include <coconut-tools/utils/Range.hpp>

#include "foliage/GrassActor.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::world;

using namespace std::string_literals;

namespace /* anonymous */ {

renderer::ActorSharedPtr createGridActor() {
	return std::make_unique<renderer::Actor>("terrain");
}

renderer::ModelSharedPtr createGridModel(
	milk::graphics::Renderer& graphicsRenderer,
	renderer::shader::PassFactory& passFactory,
	const milk::FilesystemContext& fs,
	const Heightmap& heightmap
	)
{
	auto submeshes = Mesh::Submeshes();

	const auto cellsPerPatch = 64u; // TODO: hardcoded
	const auto patchColumns = (std::get<0>(heightmap.cellCount()) - 1) / cellsPerPatch;
	const auto patchRows = (std::get<1>(heightmap.cellCount()) - 1) / cellsPerPatch;

	// TODO: vertices could be generated in vertex shader?
	const auto totalVertices = patchColumns * patchRows;
	const auto halfWidth = heightmap.width() * 0.5f;
	const auto halfDepth = heightmap.depth() * 0.5f;
	const auto patchWidth = heightmap.width() / patchRows;
	const auto patchDepth = heightmap.depth() / patchColumns;

	auto vertices = Submesh::Vertices();
	vertices.reserve(totalVertices);
	for (const auto rowIndex : coconut_tools::range(size_t(0), patchRows)) {
		const auto z = halfDepth - rowIndex * patchDepth;
		for (const auto columnIndex : coconut_tools::range(size_t(0), patchColumns)) {
			const auto x = -halfWidth + columnIndex * patchWidth;

			auto vertex = Submesh::Vertex();
			vertex.position = Position(x, 0.0f, z);
			vertex.textureCoordinate = TextureCoordinate(
				(x + halfWidth) / heightmap.width(),
				(z + halfDepth) / heightmap.depth()
			);

			vertices.emplace_back(std::move(vertex));
		}
	}
	assert(vertices.size() == totalVertices);

	const auto totalFaces = (patchColumns - 1) * (patchRows - 1);
	
	auto indices = Submesh::Indices();
	indices.reserve(totalFaces * 4u);
	for (const auto rowIndex : coconut_tools::range(size_t(0u), patchRows - 1)) {
		for (const auto columnIndex : coconut_tools::range(size_t(0u), patchColumns - 1)) {
			indices.emplace_back(rowIndex * patchColumns + columnIndex);
			indices.emplace_back(rowIndex * patchColumns + columnIndex + 1);
			indices.emplace_back((rowIndex + 1) * patchColumns + columnIndex);
			indices.emplace_back((rowIndex + 1) * patchColumns + columnIndex + 1);
		}
	}
	assert(indices.size() == totalFaces * 4u);

	submeshes.emplace_back(
		std::move(vertices),
		std::move(indices),
		"terrain"s,
		milk::graphics::PrimitiveTopology::PATCH_LIST_4_CONTROL_POINTS
		);

	auto materialConfiguration = MaterialConfiguration();

	auto renderStateConfiguration = milk::graphics::RenderState::Configuration();
	renderStateConfiguration.cullMode = milk::graphics::RenderState::CullMode::BACK;
	renderStateConfiguration.fillMode = milk::graphics::RenderState::FillMode::SOLID;//WIREFRAME;
	renderStateConfiguration.frontCounterClockwise = false;

	materialConfiguration.passType() = MaterialConfiguration::PassType::OPAQUE;
	materialConfiguration.shaderName() = "terrain"s;
	materialConfiguration.renderStateConfiguration() = renderStateConfiguration;
	materialConfiguration.properties().emplace(MaterialConfiguration::AMBIENT_COLOUR_PROPERTY, Colour(1.0f, 1.0f, 1.0f));
	materialConfiguration.properties().emplace(MaterialConfiguration::DIFFUSE_COLOUR_PROPERTY, Colour(1.0f, 1.0f, 1.0f));
	materialConfiguration.properties().emplace(MaterialConfiguration::SPECULAR_COLOUR_PROPERTY, Colour(0.0f, 0.0f, 0.0f));
	materialConfiguration.properties().emplace(MaterialConfiguration::SPECULAR_EXPONENT_PROPERTY, 0.0f);

	auto model = std::make_unique<renderer::Model>(
		"terrain"s,
		Mesh(std::move(submeshes), { { "terrain"s, std::move(materialConfiguration) } }),
		graphicsRenderer,
		passFactory,
		fs
		);

	return model;
}

} // anonymous namespace

renderer::shader::ReflectiveInterface<Terrain>::ReflectiveInterface() {
	// TODO: hard-coded
	emplaceField("minTesselationDistance", 100.0f);
	emplaceField("maxTesselationDistance", 500.0f);
	emplaceField("minTesselationExponent", 1.0f);
	emplaceField("maxTesselationExponent", 6.0f);

	emplaceMethod("cellEdgeLength", [](const Terrain& terrain) { return terrain.heightmap_.cellEdgeLength(); });
	emplaceMethod("width", [](const Terrain& terrain) { return terrain.heightmap_.width(); });
	emplaceMethod("depth", [](const Terrain& terrain) { return terrain.heightmap_.width(); });

	emplaceMethod("heightmap", [](const Terrain& terrain) { return terrain.heightmap_.texture(); });
	emplaceMethod("heightmapSampler", [](const Terrain& terrain) { return terrain.heightmap_.sampler(); });
}

Terrain::Terrain(
	milk::graphics::Renderer& graphicsRenderer,
	renderer::Scene& scene,
	renderer::shader::PassFactory& passFactory,
	renderer::ModelFactory& modelFactory,
	const milk::FilesystemContext& fs
	) :
	heightmap_(graphicsRenderer, fs)
{
	scene.add(createGridActor(), createGridModel(graphicsRenderer, passFactory, fs, heightmap_));

	foliage::GrassActor::registerShaderInputElements(passFactory.inputFactory().inputElementFactory());
	foliage::GrassActor::registerModels(modelFactory, heightmap_);

	auto grassModel = modelFactory.create("grass-fakeinst", graphicsRenderer, passFactory, fs);	
	for (float x = -20.0f; x < 20.0f; x += 20.0f) {
		for (float z = -20.0f; z < 100.0f; z += 20.0f) {
			auto grassActor = std::make_shared<pulp::world::foliage::GrassActor>(pulp::Vec3{ x, 0.0, z });
			// scene.add(grassActor, grassModel);
		}
	}
}

void Terrain::bindShaderProperties(
	renderer::shader::Properties& properties,
	std::string objectId
	) const
{
	properties.bind(std::move(objectId), renderer::shader::makeReflectiveObject(*this));
}
