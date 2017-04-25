#include "GrassActor.hpp"

#include <coconut-tools/utils/Range.hpp>

#include "coconut/pulp/renderer/shader/CallbackParameter.hpp"
#include "coconut/pulp/renderer/CommandBuffer.hpp"
#include "coconut/pulp/renderer/PassContext.hpp"
#include "coconut/pulp/mesh/Mesh.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::world;
using namespace coconut::pulp::world::foliage;

using namespace std::string_literals;

namespace /* anonymous */ {

std::unique_ptr<renderer::shader::Input::Element> createGrassPatchPositionInputElement(
	const renderer::shader::InputElementFactoryInstanceDetails& instanceDetails)
{
	return std::make_unique<renderer::shader::Input::Element>(
		instanceDetails.semantic,
		instanceDetails.semanticIndex,
		instanceDetails.format,
		renderer::shader::Input::SlotType::PER_INSTANCE_DATA,
		1,
		[](void* buffer, const void* input) {
			const auto& actor = *reinterpret_cast<const GrassActor*>(input);
			std::memcpy(buffer, &actor.patchPosition(), sizeof(Vec3));
		}
		);
}

std::unique_ptr<renderer::shader::Parameter> createGrassPatchPositionParameter(
	const renderer::shader::ParameterFactoryInstanceDetails& instanceDetails)
{
	if (instanceDetails.arraySize > 0) {
		throw std::runtime_error("grass_patch_position can't be an array");
	}

	return std::make_unique<renderer::shader::CallbackParameter<renderer::Actor, Vec3>>(
		[](Vec3& result, const renderer::Actor& actor, size_t arrayIndex) {
				assert(arrayIndex == 0);
				const auto& grassActor = dynamic_cast<const GrassActor&>(actor);
				result = grassActor.patchPosition();
			},
			instanceDetails.padding
			);
}

std::unique_ptr<renderer::Model> createGrassFakeinstModel(
	const std::string& id,
	milk::graphics::Renderer& graphicsRenderer,
	renderer::shader::PassFactory& passFactory,
	const milk::fs::FilesystemContext& filesystemContext
	)
{
	auto submeshes = Mesh::Submeshes();

	// TODO: parametrise number of blades
	static const auto BLADES = 200u * 200u;
	// static const auto BLADES = 10u * 10u;
	// static const auto VERTICES_PER_BLADE = 4u;
	// static const auto INDICES_PER_BLADE = 6u;
	auto indices = Submesh::Indices();
	indices.reserve(BLADES * INDICES_PER_BLADE);

	for (const auto bladeIndex : coconut_tools::range(0u, BLADES)) {
		const auto baseIndex = VERTICES_PER_BLADE * bladeIndex;

		indices.emplace_back(baseIndex);
		indices.emplace_back(baseIndex + 1);
		indices.emplace_back(baseIndex + 2);

		indices.emplace_back(baseIndex + 2);
		indices.emplace_back(baseIndex + 3);
		indices.emplace_back(baseIndex);
	}

	submeshes.emplace_back(
		Submesh::Vertices(),
		std::move(indices),
		"blade"s,
		milk::graphics::PrimitiveTopology::TRIANGLE_LIST
		);

	auto materialConfiguration = MaterialConfiguration();

	auto renderStateConfiguration = milk::graphics::RenderState::Configuration();
	renderStateConfiguration.cullMode = milk::graphics::RenderState::CullMode::NONE;
	renderStateConfiguration.fillMode = milk::graphics::RenderState::FillMode::SOLID;
	renderStateConfiguration.frontCounterClockwise = false;

	auto samplerConfiguration = milk::graphics::Sampler::Configuration();
	samplerConfiguration.addressModeU = milk::graphics::Sampler::AddressMode::WRAP;
	samplerConfiguration.addressModeV = milk::graphics::Sampler::AddressMode::WRAP;
	samplerConfiguration.addressModeW = milk::graphics::Sampler::AddressMode::WRAP;
	samplerConfiguration.filter = milk::graphics::Sampler::Filter::MIN_MAG_MIP_POINT;

	// TODO: shouldn't be hardcoded
	materialConfiguration.passType() = MaterialConfiguration::PassType::OPAQUE;
	materialConfiguration.shaderName() = "grass-fakeinst"s;
	materialConfiguration.renderStateConfiguration() = renderStateConfiguration;
	materialConfiguration.addTexture(
		MaterialConfiguration::NOISE_MAP_TEXTURE,
		filesystemContext.makeAbsolute("data/models/noise.tga"),
		samplerConfiguration
		);
	materialConfiguration.properties().emplace(MaterialConfiguration::AMBIENT_COLOUR_PROPERTY, Colour(0.086f, 0.356f, 0.192f));
	materialConfiguration.properties().emplace(MaterialConfiguration::DIFFUSE_COLOUR_PROPERTY, Colour(0.086f, 0.356f, 0.192f));
	materialConfiguration.properties().emplace(MaterialConfiguration::SPECULAR_COLOUR_PROPERTY, Colour(0.05f, 0.05f, 0.05f));
	materialConfiguration.properties().emplace(MaterialConfiguration::SPECULAR_EXPONENT_PROPERTY, 1.5f);

	return std::make_unique<renderer::Model>(
		id,
		Mesh(std::move(submeshes), { { "blade", std::move(materialConfiguration) } }),
		graphicsRenderer,
		passFactory,
		filesystemContext
		);
}

} // anonymous namespace

void GrassActor::registerShaderInputElements(renderer::shader::InputElementFactory& inputElementFactory) {
	auto instanceDetails = renderer::shader::InputElementFactoryInstanceDetails("GRASS_PATCH_POSITION", 0);
	if (!inputElementFactory.isCreatorRegistered(instanceDetails)) {
		inputElementFactory.registerCreator(
			instanceDetails,
			&createGrassPatchPositionInputElement
			);
	}
}

void GrassActor::registerParameters(renderer::shader::ParameterFactory& parameterFactory) {
	auto instanceDetails = renderer::shader::ParameterFactoryInstanceDetails("grass_patch_position");
	if (!parameterFactory.isCreatorRegistered(instanceDetails)) {
		parameterFactory.registerCreator(
			instanceDetails,
			&createGrassPatchPositionParameter
		);
	}
}

void GrassActor::registerModels(renderer::ModelFactory& modelFactory) {
	const auto name = "grass-fakeinst";
	if (!modelFactory.hasGenerator(name)) {
		modelFactory.registerGenerator(name, &createGrassFakeinstModel);
	}
}

GrassActor::GrassActor(const math::Vec3& patchPosition) :
	Actor("grass"),
	patchPosition_(patchPosition)
{
}
