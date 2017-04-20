#include "GrassActor.hpp"

#include <coconut-tools/utils/Range.hpp>

#include "coconut/pulp/renderer/shader/CallbackParameter.hpp"
#include "coconut/pulp/renderer/CommandBuffer.hpp"
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

GrassActor::GrassActor(const math::Vec3& patchPosition) :
	Actor("grass"),
	patchPosition_(patchPosition)
{
}
