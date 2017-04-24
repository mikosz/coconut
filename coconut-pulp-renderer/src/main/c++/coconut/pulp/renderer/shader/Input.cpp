#include "Input.hpp"

#include <algorithm>
#include <iterator>
#include <numeric>

#include <boost/range/adaptor/filtered.hpp>

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;
using namespace coconut::pulp::renderer::shader;

namespace /* anonymous */ {

milk::graphics::InputLayout::Elements createInputLayoutElements(const Input::Elements& elements) {
	auto inputLayoutElements = milk::graphics::InputLayout::Elements();

	std::copy(elements.begin(), elements.end(), std::back_inserter(inputLayoutElements));

	return inputLayoutElements;
}

size_t elementSize(const Input::Elements& elements, Input::SlotType slotType) {
	return std::accumulate(
		elements.begin(),
		elements.end(),
		static_cast<size_t>(0),
		[slotType](size_t sum, const auto& element) {
			if (slotType == element.inputSlotType) {
				sum += formatSize(element.format);
			}
			return sum;
		}
	);
}

} // anonymous namespace

Input::Element::Element(
	std::string semantic,
	size_t semanticIndex,
	milk::graphics::PixelFormat format,
	SlotType inputSlotType,
	size_t instanceDataStepRate,
	WriteFunc writeFunc
	) :
	milk::graphics::InputLayout::Element(
		std::move(semantic),
		semanticIndex,
		format,
		inputSlotType,
		instanceDataStepRate
		),
	writeFunc(std::move(writeFunc))
{
}

Input::Input(milk::graphics::Renderer& graphicsRenderer, Elements elements) :
	elements_(std::move(elements)),
	layout_(graphicsRenderer, createInputLayoutElements(elements_))
{
}

size_t Input::vertexSize() const noexcept {
	return elementSize(elements_, SlotType::PER_VERTEX_DATA);
}

void* Input::writeVertex(void* buffer, const void* input) const
{
	auto* target = reinterpret_cast<std::uint8_t*>(buffer);
	for (const auto& element : elements_ | boost::adaptors::filtered(
		[](const auto& element) {
			return element.inputSlotType == SlotType::PER_VERTEX_DATA;
		}))
	{
		element.writeFunc(target, input);
		target += milk::graphics::formatSize(element.format);
	}
	return target;
}

size_t Input::instanceSize() const noexcept {
	return elementSize(elements_, SlotType::PER_INSTANCE_DATA);
}

void* Input::writeInstance(void* buffer, const Actor& actor) const {
	auto* target = reinterpret_cast<std::uint8_t*>(buffer); // TODO: duplicated code
	for (const auto& element : elements_ | boost::adaptors::filtered(
		[](const auto& element) {
			return element.inputSlotType == SlotType::PER_INSTANCE_DATA;
		}))
	{
		element.writeFunc(target, &actor);
		target += milk::graphics::formatSize(element.format);
	}
	return target;
}
