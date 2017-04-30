#include "Terrain.hpp"

#include <coconut-tools/utils/Range.hpp>

#include "coconut/pulp/renderer/shader/CallbackParameter.hpp"

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

	model->material().setTexture("heightmap"s, std::make_tuple(heightmap.texture(), heightmap.sampler()));

	return model;
}

std::unique_ptr<renderer::shader::Parameter> createMinTesselationDistanceParameter(
	const renderer::shader::ParameterFactoryInstanceDetails& instanceDetails)
{
	// TODO: actor doesn't make sense as an argument here
	return std::make_unique<renderer::shader::CallbackParameter<renderer::Actor, float>>(
		[](float& result, const renderer::Actor&, size_t arrayIndex) {
			assert(arrayIndex == 0);
			result = 100.0f;
		},
		instanceDetails.padding
		);
}

std::unique_ptr<renderer::shader::Parameter> createMaxTesselationDistanceParameter(
	const renderer::shader::ParameterFactoryInstanceDetails& instanceDetails)
{
	// TODO: actor doesn't make sense as an argument here
	return std::make_unique<renderer::shader::CallbackParameter<renderer::Actor, float>>(
		[](float& result, const renderer::Actor&, size_t arrayIndex) {
			assert(arrayIndex == 0);
			result = 500.0f;
		},
		instanceDetails.padding
		);
}

std::unique_ptr<renderer::shader::Parameter> createMinTesselationExponentParameter(
	const renderer::shader::ParameterFactoryInstanceDetails& instanceDetails)
{
	// TODO: actor doesn't make sense as an argument here
	return std::make_unique<renderer::shader::CallbackParameter<renderer::Actor, float>>(
		[](float& result, const renderer::Actor&, size_t arrayIndex) {
			assert(arrayIndex == 0);
			result = 1.0f;
		},
		instanceDetails.padding
		);
}

std::unique_ptr<renderer::shader::Parameter> createMaxTesselationExponentParameter(
	const renderer::shader::ParameterFactoryInstanceDetails& instanceDetails)
{
	// TODO: actor doesn't make sense as an argument here
	return std::make_unique<renderer::shader::CallbackParameter<renderer::Actor, float>>(
		[](float& result, const renderer::Actor&, size_t arrayIndex) {
			assert(arrayIndex == 0);
			result = 6.0f;
		},
		instanceDetails.padding
		);
}

std::unique_ptr<renderer::shader::Parameter> createCellEdgeLengthParameter(
	const renderer::shader::ParameterFactoryInstanceDetails& instanceDetails)
{
	// TODO: actor doesn't make sense as an argument here
	return std::make_unique<renderer::shader::CallbackParameter<renderer::Actor, float>>(
		[](float& result, const renderer::Actor&, size_t arrayIndex) {
			assert(arrayIndex == 0);
			result = 1.0f;
		},
		instanceDetails.padding
		);
}

std::unique_ptr<renderer::shader::Parameter> createWidthParameter(
	const renderer::shader::ParameterFactoryInstanceDetails& instanceDetails)
{
	// TODO: actor doesn't make sense as an argument here
	return std::make_unique<renderer::shader::CallbackParameter<renderer::Actor, float>>(
		[](float& result, const renderer::Actor&, size_t arrayIndex) {
			assert(arrayIndex == 0);
			result = 257.0f; // TODO: HARDCODED!
		},
		instanceDetails.padding
		);
}

std::unique_ptr<renderer::shader::Parameter> createDepthParameter(
	const renderer::shader::ParameterFactoryInstanceDetails& instanceDetails)
{
	// TODO: actor doesn't make sense as an argument here
	return std::make_unique<renderer::shader::CallbackParameter<renderer::Actor, float>>(
		[](float& result, const renderer::Actor&, size_t arrayIndex) {
			assert(arrayIndex == 0);
			result = 257.0f; // TODO: HARDCODED!
		},
		instanceDetails.padding
		);
}

std::unique_ptr<renderer::shader::Resource> createHeightmapResource(milk::graphics::ShaderType shaderType, size_t slot) {
	return std::make_unique<renderer::shader::DataResource>(
		[](const renderer::PassContext& passContext) -> const milk::graphics::Texture* {
			return &std::get<milk::graphics::Texture2d>(passContext.material->texture("heightmap"));
		},
		shaderType,
		slot
		);
}

// TODO: duplicated code, generate these using property keys
std::unique_ptr<renderer::shader::Resource> createHeightmapSamplerResource(milk::graphics::ShaderType shaderType, size_t slot) {
	return std::make_unique<renderer::shader::SamplerResource>(
		[](const renderer::PassContext& passContext) -> const milk::graphics::Sampler* {
			return &std::get<milk::graphics::Sampler>(passContext.material->texture("heightmap"));
		},
		shaderType,
		slot
		);
}

} // anonymous namespace

Terrain::Terrain(
	milk::graphics::Renderer& graphicsRenderer,
	renderer::Scene& scene,
	renderer::shader::PassFactory& passFactory,
	renderer::ModelFactory& modelFactory,
	const milk::FilesystemContext& fs
	) :
	heightmap_(graphicsRenderer, fs)
{
	// TODO: refactor. want an easier way to do this
	// TODO: perhaps have just some TerrainProperties object registered, and put these values there?
	auto& parameterFactory = passFactory.shaderFactory().parameterFactory();

	{
		auto instanceDetails = renderer::shader::ParameterFactoryInstanceDetails("terrain_min_tesselation_distance");
		if (!parameterFactory.isCreatorRegistered(instanceDetails)) {
			parameterFactory.registerCreator(
				instanceDetails,
				&createMinTesselationDistanceParameter
			);
		}
	}

	{
		auto instanceDetails = renderer::shader::ParameterFactoryInstanceDetails("terrain_max_tesselation_distance");
		if (!parameterFactory.isCreatorRegistered(instanceDetails)) {
			parameterFactory.registerCreator(
				instanceDetails,
				&createMaxTesselationDistanceParameter
			);
		}
	}

	{
		auto instanceDetails = renderer::shader::ParameterFactoryInstanceDetails("terrain_min_tesselation_exponent");
		if (!parameterFactory.isCreatorRegistered(instanceDetails)) {
			parameterFactory.registerCreator(
				instanceDetails,
				&createMinTesselationExponentParameter
			);
		}
	}

	{
		auto instanceDetails = renderer::shader::ParameterFactoryInstanceDetails("terrain_max_tesselation_exponent");
		if (!parameterFactory.isCreatorRegistered(instanceDetails)) {
			parameterFactory.registerCreator(
				instanceDetails,
				&createMinTesselationDistanceParameter
			);
		}
	}

	{
		auto instanceDetails = renderer::shader::ParameterFactoryInstanceDetails("terrain_cell_edge_length");
		if (!parameterFactory.isCreatorRegistered(instanceDetails)) {
			parameterFactory.registerCreator(
				instanceDetails,
				&createCellEdgeLengthParameter
			);
		}
	}

	{
		auto instanceDetails = renderer::shader::ParameterFactoryInstanceDetails("terrain_width");
		if (!parameterFactory.isCreatorRegistered(instanceDetails)) {
			parameterFactory.registerCreator(
				instanceDetails,
				&createWidthParameter
			);
		}
	}

	{
		auto instanceDetails = renderer::shader::ParameterFactoryInstanceDetails("terrain_depth");
		if (!parameterFactory.isCreatorRegistered(instanceDetails)) {
			parameterFactory.registerCreator(
				instanceDetails,
				&createDepthParameter
			);
		}
	}

	auto& resourceFactory = passFactory.shaderFactory().resourceFactory();

	if (!resourceFactory.isCreatorRegistered("heightmap")) {
		resourceFactory.registerCreator("heightmap", &createHeightmapResource);
	}

	if (!resourceFactory.isCreatorRegistered("heightmap_sampler")) {
		resourceFactory.registerCreator("heightmap_sampler", &createHeightmapSamplerResource);
	}

	scene.add(createGridActor(), createGridModel(graphicsRenderer, passFactory, fs, heightmap_));

	foliage::GrassActor::registerShaderInputElements(passFactory.inputFactory().inputElementFactory());
	foliage::GrassActor::registerParameters(parameterFactory);
	foliage::GrassActor::registerModels(modelFactory, heightmap_);

	auto grassModel = modelFactory.create("grass-fakeinst", graphicsRenderer, passFactory, fs);	
	for (float x = -20.0f; x < 20.0f; x += 20.0f) {
		for (float z = -20.0f; z < 100.0f; z += 20.0f) {
			auto grassActor = std::make_shared<pulp::world::foliage::GrassActor>(pulp::Vec3{ x, 0.0, z });
			scene.add(grassActor, grassModel);
		}
	}
}
