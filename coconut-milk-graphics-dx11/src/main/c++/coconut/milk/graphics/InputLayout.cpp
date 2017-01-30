#include "InputLayout.hpp"

#include <iterator>

#include "DirectXError.hpp"
#include "Renderer.hpp"
#include "compile-shader.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::graphics;

namespace /* anonymous */ {

size_t inputSlotIndex(InputLayout::SlotType inputSlot) {
	switch (inputSlot) {
	case InputLayout::SlotType::PER_VERTEX_DATA:
		return 0u;
	case InputLayout::SlotType::PER_INSTANCE_DATA:
		return 1u;
	default:
		assert(!"Unexpected input slot");
		return 0u;
	}
}

system::COMWrapper<ID3D11InputLayout> createLayout(
	const InputLayout::Elements& elements,
	Renderer& renderer,
	std::vector<std::uint8_t> shaderData
	)
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> descs;
	descs.reserve(elements.size());

	for (const auto& element : elements) {
		descs.emplace_back();
		auto& desc = descs.back();

		std::memset(&desc, 0, sizeof(desc));

		desc.SemanticName = element.semantic.c_str();
		desc.SemanticIndex = static_cast<UINT>(element.semanticIndex);
		desc.Format = static_cast<DXGI_FORMAT>(element.format);
		desc.InputSlot = static_cast<UINT>(inputSlotIndex(element.inputSlotType));
		desc.InputSlotClass = static_cast<D3D11_INPUT_CLASSIFICATION>(element.inputSlotType);
		desc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	}

	system::COMWrapper<ID3D11InputLayout> layout;
	checkDirectXCall(
		renderer.internalDevice().CreateInputLayout(
			&descs.front(),
			static_cast<UINT>(descs.size()),
			shaderData.data(),
			shaderData.size(),
			&layout.get()
			),
		"Failed to create an input layout"
		);

	return layout;
}

std::vector<std::uint8_t> createDummyVertexShader(const InputLayout::Elements& elements) {
	const auto* const prefix = "struct VIn {\n";
	const auto* const suffix = 
		"};\n"
		"\n"
		"SV_POSITION main(VIn in) { return (0.0f).xxxx; }\n"
		;

	std::ostringstream shaderTextStream;
	shaderTextStream << prefix;

	for (const auto& element : elements) {
		shaderTextStream
			<< "\t"
			<< formatHLSLType(element.format)
			<< element.semantic << "_" << element.semanticIndex
			<< " : "
			<< element.semantic
			<< ";\n";
	}

	std::vector<std::uint8_t> shaderData;
	const auto shaderText = shaderTextStream.str();
	shaderData.reserve(shaderText.size());
	std::copy(shaderText.begin(), shaderText.end(), std::back_inserter(shaderData));

	return compileShader(shaderData, "main");
}

} // anonymous namespace

InputLayout::Element::Element(
	std::string semantic,
	size_t semanticIndex,
	PixelFormat format,
	SlotType inputSlotType,
	size_t instanceDataStepRate
	) :
	semantic(std::move(semantic)),
	semanticIndex(semanticIndex),
	format(format),
	inputSlotType(inputSlotType),
	instanceDataStepRate(instanceDataStepRate)
{
}

InputLayout::InputLayout(
	Renderer& renderer,
	const Elements& elements
	) :
	dxInputLayout_(createLayout(elements, renderer, createDummyVertexShader(elements)))
{
}
