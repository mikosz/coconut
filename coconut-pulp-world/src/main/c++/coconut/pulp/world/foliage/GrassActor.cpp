#include "GrassActor.hpp"

#include <coconut-tools/utils/Range.hpp>

#include "coconut/pulp/renderer/shader/ReflectiveObject.hpp"
#include "coconut/pulp/renderer/CommandBuffer.hpp"
#include "coconut/pulp/renderer/PassContext.hpp"
#include "coconut/pulp/mesh/Mesh.hpp"
#include "coconut/pulp/world/Heightmap.hpp"

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

std::unique_ptr<renderer::Model> createGrassFakeinstModel(
	const std::string& id,
	milk::graphics::Renderer& graphicsRenderer,
	renderer::shader::PassFactory& passFactory,
	const milk::fs::FilesystemContext& filesystemContext,
	const Heightmap& heightmap
	)
{
	auto submeshes = Mesh::Submeshes();

	// TODO: parametrise number of blades
	static const auto BLADES = 400u * 400u;
	static const auto SEGMENTS_PER_BLADE = 4u;
	static const auto VERTICES_PER_BLADE = 2u * (SEGMENTS_PER_BLADE + 1);
	static const auto INDICES_PER_BLADE = 6u * SEGMENTS_PER_BLADE;
	auto indices = Submesh::Indices();
	//indices.reserve(BLADES * INDICES_PER_BLADE);
	indices.reserve(BLADES);

	for (const auto bladeIndex : coconut_tools::range(0u, BLADES)) {
		//const auto baseIndex = VERTICES_PER_BLADE * bladeIndex;

		//for (const auto segmentIndex : coconut_tools::range(0u, SEGMENTS_PER_BLADE)) {
		//	const auto baseSegmentIndex = baseIndex + (segmentIndex * 2u);
		//	indices.emplace_back(baseSegmentIndex);
		//	indices.emplace_back(baseSegmentIndex + 2);
		//	indices.emplace_back(baseSegmentIndex + 1);

		//	indices.emplace_back(baseSegmentIndex + 1);
		//	indices.emplace_back(baseSegmentIndex + 2);
		//	indices.emplace_back(baseSegmentIndex + 3);			
		//}
		indices.emplace_back(bladeIndex);
	}

	submeshes.emplace_back(
		Submesh::Vertices(),
		std::move(indices),
		"blade"s,
		//milk::graphics::PrimitiveTopology::TRIANGLE_LIST
		milk::graphics::PrimitiveTopology::POINT_LIST
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
	//materialConfiguration.shaderName() = "grass-fakeinst"s;
	materialConfiguration.shaderName() = "grass-fakeinst-g"s;
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

	auto model = std::make_unique<renderer::Model>(
		id,
		Mesh(std::move(submeshes), { { "blade", std::move(materialConfiguration) } }),
		graphicsRenderer,
		passFactory,
		filesystemContext
		);

	model->material().setTexture("heightmap"s, std::make_tuple(heightmap.texture(), heightmap.sampler()));

	return std::move(model);
}

} // anonymous namespace

template <>
class renderer::shader::ReflectiveInterface<GrassActor> :
	public renderer::shader::ReflectiveInterfaceBase<GrassActor>
{
public:

	ReflectiveInterface() {
		// TODO: don't like taking address of returned value here. Maybe extend size of Property?
		// TODO: OR make ReflectiveInterface GrassActor's friend and return address of field?
		emplaceMethod("grassPatchPosition", [](const GrassActor& grassActor) { return &grassActor.patchPosition(); });
	}

};

void GrassActor::registerShaderInputElements(renderer::shader::InputElementFactory& inputElementFactory) {
	auto instanceDetails = renderer::shader::InputElementFactoryInstanceDetails("GRASS_PATCH_POSITION", 0);
	if (!inputElementFactory.isCreatorRegistered(instanceDetails)) {
		inputElementFactory.registerCreator(
			instanceDetails,
			&createGrassPatchPositionInputElement
			);
	}
}

void GrassActor::registerModels(renderer::ModelFactory& modelFactory, const Heightmap& heightmap) {
	const auto name = "grass-fakeinst";
	if (!modelFactory.hasGenerator(name)) {
		modelFactory.registerGenerator(
			name,
			[&heightmap]( // TODO: this is obviously super-temp
				const std::string& id,
				milk::graphics::Renderer& graphicsRenderer,
				renderer::shader::PassFactory& passFactory,
				const milk::fs::FilesystemContext& filesystemContext
				)
			{
				return createGrassFakeinstModel(id, graphicsRenderer, passFactory, filesystemContext, heightmap);
			}
			);
	}
}

void GrassActor::bindShaderProperties(
	renderer::shader::Properties& properties,
	std::string objectId
	) const
{
	properties.bind(std::move(objectId), renderer::shader::makeReflectiveObject(*this));
}

GrassActor::GrassActor(const math::Vec3& patchPosition) :
	Actor("grass"),
	patchPosition_(patchPosition)
{
}
