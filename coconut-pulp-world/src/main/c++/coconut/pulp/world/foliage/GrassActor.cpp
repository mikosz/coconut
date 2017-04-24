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

std::unique_ptr<renderer::shader::Resource> createGrassPatchPositionsResource(milk::graphics::ShaderType shaderType, size_t slot) {
	return std::make_unique<renderer::shader::DataResource>(
		[](const renderer::PassContext& passContext) -> const milk::graphics::Resource* {
			return &GrassActor::grassPatchPositionsResource(*passContext.graphicsRenderer);
		},
		shaderType,
		slot
		);
}

} // anonymous namespace

std::vector<math::Vec3> GrassActor::allPatchPositions_;

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

void GrassActor::registerResources(renderer::shader::ResourceFactory& resourceFactory) {
	const auto instanceDetails = "grass_patch_positions"s;
	if (!resourceFactory.isCreatorRegistered(instanceDetails)) {
		resourceFactory.registerCreator(
			instanceDetails,
			&createGrassPatchPositionsResource
		);
	}
}

const milk::graphics::Resource& GrassActor::grassPatchPositionsResource(milk::graphics::Renderer& graphicsRenderer) {
	static auto initialised = false;
	static auto buffer = milk::graphics::Buffer();
	if (!initialised) {
		auto configuration = milk::graphics::Buffer::Configuration();
		configuration.size = GrassActor::allPatchPositions_.size() * sizeof(Vec3);
		configuration.stride = sizeof(Vec3);
		configuration.allowModifications = false;
		configuration.allowCPURead = false;
		configuration.allowGPUWrite = false;
		configuration.elementFormat = milk::graphics::PixelFormat::R32G32B32_FLOAT;

		buffer = milk::graphics::Buffer( // TODO: add initialise? have unified approach with Texture
			graphicsRenderer,
			milk::graphics::Buffer::CreationPurpose::SHADER_RESOURCE,
			configuration,
			GrassActor::allPatchPositions_.data()
			);

		initialised = true;
	}

	return buffer;
}

GrassActor::GrassActor(const math::Vec3& patchPosition) :
	Actor("grass"),
	patchPosition_(patchPosition)
{
	allPatchPositions_.emplace_back(patchPosition);
}
