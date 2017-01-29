#include "InputLayoutDescription.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::graphics;

InputLayoutDescription::Element::Element(
	std::string semantic,
	size_t semanticIndex,
	PixelFormat format,
	SlotType inputSlotType,
	size_t instanceDataStepRate,
	VertexDataCallback vertexDataCallback
	) :
	semantic(std::move(semantic)),
	semanticIndex(semanticIndex),
	format(format),
	inputSlotType(inputSlotType),
	instanceDataStepRate(instanceDataStepRate),
	vertexDataCallback(vertexDataCallback)
{
}

InputLayoutDescription::InputLayoutDescription(Elements elements) :
	elements_(std::move(elements))
{
}
