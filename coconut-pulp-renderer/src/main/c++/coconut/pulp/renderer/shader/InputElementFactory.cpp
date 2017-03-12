#include "InputElementFactory.hpp"

#include <coconut-tools/logger.hpp>

#include "coconut/pulp/mesh/Submesh.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;
using namespace coconut::pulp::renderer::shader;

namespace /* anonymous */ {

CT_LOGGER_CATEGORY("COCONUT.PULP.RENDERER.SHADER.INPUT_ELEMENT_FACTORY");

// TODO: these creators could be limited to the lambda, other parameters
// are the same everywhere
std::unique_ptr<Input::Element> createPositionElement(
	const InputElementFactoryInstanceDetails& instanceDetails)
{
	return std::make_unique<Input::Element>(
		instanceDetails.semantic,
		instanceDetails.semanticIndex,
		instanceDetails.format,
		Input::SlotType::PER_VERTEX_DATA,
		0,
		[pixelFormat=instanceDetails.format](void* buffer, const void* input) {
			const auto& vertex = *reinterpret_cast<const mesh::Submesh::Vertex*>(input);
			vertex.position.storeAs(buffer, pixelFormat);
		}
		);
}

std::unique_ptr<Input::Element> createNormalElement(
	const InputElementFactoryInstanceDetails& instanceDetails)
{
	return std::make_unique<Input::Element>(
		instanceDetails.semantic,
		instanceDetails.semanticIndex,
		instanceDetails.format,
		Input::SlotType::PER_VERTEX_DATA,
		0,
		[pixelFormat=instanceDetails.format](void* buffer, const void* input) {
			const auto& vertex = *reinterpret_cast<const mesh::Submesh::Vertex*>(input);
			vertex.normal.storeAs(buffer, pixelFormat);
		}
		);
}

std::unique_ptr<Input::Element> createTexcoordElement(
	const InputElementFactoryInstanceDetails& instanceDetails)
{
	return std::make_unique<Input::Element>(
		instanceDetails.semantic,
		instanceDetails.semanticIndex,
		instanceDetails.format,
		Input::SlotType::PER_VERTEX_DATA,
		0,
		[pixelFormat=instanceDetails.format](void* buffer, const void* input) {
			const auto& vertex = *reinterpret_cast<const mesh::Submesh::Vertex*>(input);
			vertex.textureCoordinate.storeAs(buffer, pixelFormat);
		}
		);
}


std::unique_ptr<Input::Element> createPatchPositionElement(
	const InputElementFactoryInstanceDetails& instanceDetails)
{
	return std::make_unique<Input::Element>(
		instanceDetails.semantic,
		instanceDetails.semanticIndex,
		instanceDetails.format,
		Input::SlotType::PER_INSTANCE_DATA,
		1, // TODO: instance step rate should be deduced somehow
		[](void* buffer, const void* input) {
			assert(false);
			//auto* target = reinterpret_cast<float*>(buffer);
			//const auto& instance = *reinterpret_cast<const model::Data::Instance*>(input);
			//const auto& patchPosition = instance.patchPosition;

			//target[0] = patchPosition.x();
			//target[1] = patchPosition.y();
			//target[2] = patchPosition.z();
			//target[3] = 1.0f; // TODO
		}
		);
}

} // anonymous namespace

bool coconut::pulp::renderer::shader::operator==(
	const InputElementFactoryInstanceDetails& lhs, const InputElementFactoryInstanceDetails& rhs)
{
	return
		lhs.semantic == rhs.semantic &&
		lhs.semanticIndex == rhs.semanticIndex;
}

std::ostream& coconut::pulp::renderer::shader::operator<<(
	std::ostream& os, const InputElementFactoryInstanceDetails& instanceDetails)
{
	return os
		<< instanceDetails.semantic
		<< ':' << instanceDetails.semanticIndex;
}

detail::InputElementCreator::InputElementCreator() {
	registerBuiltins();
}

void detail::InputElementCreator::registerBuiltins() {
	using milk::graphics::PixelFormat;
	using InstanceDetails = InputElementFactoryInstanceDetails;

	registerCreator(InstanceDetails("POSITION", 0), &createPositionElement);
	registerCreator(InstanceDetails("NORMAL", 0), &createNormalElement);
	registerCreator(InstanceDetails("TEXCOORD", 0), &createTexcoordElement);

	registerCreator(InstanceDetails("PATCH_POSITION", 0), &createPatchPositionElement);
}
