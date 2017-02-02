#include "InputElementFactory.hpp"

#include <coconut-tools/logger.hpp>

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;
using namespace coconut::pulp::renderer::shader;

namespace /* anonymous */ {

CT_LOGGER_CATEGORY("COCONUT.PULP.RENDERER.SHADER.INPUT_ELEMENT_FACTORY");

std::unique_ptr<Input::Element> createPositionRGBAElement(
	const InputElementFactoryInstanceDetails& instanceDetails)
{
	return std::make_unique<Input::Element>(
		instanceDetails.semantic,
		instanceDetails.semanticIndex,
		instanceDetails.format,
		Input::SlotType::PER_VERTEX_DATA,
		0u,
		[](void* buffer, const void* input) {
			auto* target = reinterpret_cast<float*>(buffer);
			const auto& vertexIterator = *reinterpret_cast<const model::Data::VertexIterator*>(input);
			const auto& position =
				vertexIterator.data().positions[vertexIterator.vertexDescriptor().positionIndex];

			target[0] = position.x();
			target[1] = position.y();
			target[2] = position.z();
			target[3] = 1.0f; // TODO
		}
		);
}

std::unique_ptr<Input::Element> createNormalRGBElement(
	const InputElementFactoryInstanceDetails& instanceDetails)
{
	return std::make_unique<Input::Element>(
		instanceDetails.semantic,
		instanceDetails.semanticIndex,
		instanceDetails.format,
		Input::SlotType::PER_VERTEX_DATA,
		0u,
		[](void* buffer, const void* input) {
			auto* target = reinterpret_cast<float*>(buffer);
			const auto& vertexIterator = *reinterpret_cast<const model::Data::VertexIterator*>(input);
			const auto& normal =
				vertexIterator.data().normals[vertexIterator.vertexDescriptor().normalIndex];

			target[0] = normal.x();
			target[1] = normal.y();
			target[2] = normal.z();
		}
		);
}

std::unique_ptr<Input::Element> createTexcoordRGElement(
	const InputElementFactoryInstanceDetails& instanceDetails)
{
	return std::make_unique<Input::Element>(
		instanceDetails.semantic,
		instanceDetails.semanticIndex,
		instanceDetails.format,
		Input::SlotType::PER_VERTEX_DATA,
		0u,
		[](void* buffer, const void* input) {
			auto* target = reinterpret_cast<float*>(buffer);
			const auto& vertexIterator = *reinterpret_cast<const model::Data::VertexIterator*>(input);
			const auto& texcoord =
				vertexIterator.data().textureCoordinates[vertexIterator.vertexDescriptor().textureCoordinateIndex];

			target[0] = texcoord.x();
			target[1] = texcoord.y();
		}
		);
}

// TODO: simplify all of this
std::unique_ptr<Input::Element> createPatchPositionRGBAElement(
	const InputElementFactoryInstanceDetails& instanceDetails)
{
	return std::make_unique<Input::Element>(
		instanceDetails.semantic,
		instanceDetails.semanticIndex,
		instanceDetails.format,
		Input::SlotType::PER_INSTANCE_DATA,
		1u,
		[](void* buffer, const void* input) {
			auto* target = reinterpret_cast<float*>(buffer);
			const auto& instance = *reinterpret_cast<const model::Data::Instance*>(input);
			const auto& patchPosition = instance.patchPosition;

			target[0] = patchPosition.x();
			target[1] = patchPosition.y();
			target[2] = patchPosition.z();
			target[3] = 1.0f; // TODO
		}
		);
}

} // anonymous namespace

bool coconut::pulp::renderer::shader::operator==(
	const InputElementFactoryInstanceDetails& lhs, const InputElementFactoryInstanceDetails& rhs)
{
	return
		lhs.semantic == rhs.semantic &&
		lhs.semanticIndex == rhs.semanticIndex &&
		lhs.format == rhs.format
		;
}

std::ostream& coconut::pulp::renderer::shader::operator<<(
	std::ostream& os, const InputElementFactoryInstanceDetails& instanceDetails)
{
	return os
		<< instanceDetails.semantic
		<< ':' << instanceDetails.semanticIndex
		<< " <" << instanceDetails.format << ">";
}

detail::InputElementCreator::InputElementCreator() {
	registerBuiltins();
}

void detail::InputElementCreator::registerBuiltins() {
	using milk::graphics::PixelFormat;
	using InstanceDetails = InputElementFactoryInstanceDetails;

	registerCreator(InstanceDetails("POSITION", 0, PixelFormat::R32G32B32A32_FLOAT), &createPositionRGBAElement);
	registerCreator(InstanceDetails("NORMAL", 0, PixelFormat::R32G32B32_FLOAT), &createNormalRGBElement);
	registerCreator(InstanceDetails("TEXCOORD", 0, PixelFormat::R32G32_FLOAT), &createTexcoordRGElement);

	registerCreator(InstanceDetails("PATCH_POSITION", 0, PixelFormat::R32G32B32A32_FLOAT),
		&createPatchPositionRGBAElement);
}
