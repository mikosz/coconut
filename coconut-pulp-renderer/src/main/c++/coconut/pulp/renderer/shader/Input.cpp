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

size_t Input::vertexSize(SlotType slotType) const {
	return std::accumulate(
		elements_.begin(),
		elements_.end(),
		static_cast<size_t>(0),
		[](size_t sum, const Element& element) { return sum + formatSize(element.format); }
	);
}

void Input::writeVertex(
	void* buffer,
	const void* input,
	SlotType slotType
	) const
{
	std::uint8_t* target = reinterpret_cast<std::uint8_t*>(buffer);
	for (const auto& element : elements_ | boost::adaptors::filtered([slotType](const auto& element) {
			return element.inputSlotType == slotType;
		}))
	{
		element.writeFunc(buffer, input);
		target += milk::graphics::formatSize(element.format);
	}
}
