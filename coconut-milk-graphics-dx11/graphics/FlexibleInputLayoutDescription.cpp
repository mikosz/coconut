#include "FlexibleInputLayoutDescription.hpp"

#include "Device.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::graphics;

FlexibleInputLayoutDescription::PositionElement::PositionElement(size_t index, Format format) :
	index_(index),
	format_(format)
{
}

void FlexibleInputLayoutDescription::PositionElement::toElementDesc(D3D11_INPUT_ELEMENT_DESC* desc) {
	std::memset(desc, 0, sizeof(*desc));

	desc->SemanticName = "POSITION";
	desc->SemanticIndex = index_;
	desc->Format = static_cast<DXGI_FORMAT>(format_);
	desc->InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	desc->AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
}

system::COMWrapper<ID3D11InputLayout> FlexibleInputLayoutDescription::makeLayout(
	Device& device,
	void* shaderData,
	size_t shaderSize
	) {
	std::vector<D3D11_INPUT_ELEMENT_DESC> descs;
	descs.resize(elements_.size());

	for (size_t i = 0; i < elements_.size(); ++i) {
		elements_[i]->toElementDesc(&descs[i]);
	}

	system::COMWrapper<ID3D11InputLayout> layout;
	if (FAILED(device.d3dDevice()->CreateInputLayout(
		&descs.front(),
		descs.size(),
		shaderData,
		shaderSize,
		&layout.get()
		))) {
		throw std::runtime_error("Failed to create an input layout");
	}

	return layout;
}

void FlexibleInputLayoutDescription::push(std::shared_ptr<Element> element) {
	elements_.push_back(element);
}
