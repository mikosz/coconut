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
		[](void* buffer, const model::Data::VertexIterator& vertexIterator) {
			auto* target = reinterpret_cast<float*>(buffer);
			const auto& position = vertexIterator.data().positions[vertexIterator.index()];

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
		[](void* buffer, const model::Data::VertexIterator& vertexIterator) {
			auto* target = reinterpret_cast<float*>(buffer);
			const auto& normal = vertexIterator.data().normals[vertexIterator.index()];

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
		[](void* buffer, const model::Data::VertexIterator& vertexIterator) {
			auto* target = reinterpret_cast<float*>(buffer);
			const auto& texcoord = vertexIterator.data().textureCoordinates[vertexIterator.index()];

			target[0] = texcoord.x();
			target[1] = texcoord.y();
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
}
