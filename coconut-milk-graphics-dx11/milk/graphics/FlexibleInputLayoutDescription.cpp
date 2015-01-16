#include "FlexibleInputLayoutDescription.hpp"

#include <algorithm>
#include <numeric>

#include "DirectXError.hpp"
#include "Device.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::graphics;

namespace /* anonymous */ {

size_t formatSize(FlexibleInputLayoutDescription::Format format) {
	switch (format) {
	case FlexibleInputLayoutDescription::Format::R32G32_FLOAT:
		return 2 * 4;
	case FlexibleInputLayoutDescription::Format::R32G32B32_FLOAT:
		return 3 * 4;
	case FlexibleInputLayoutDescription::Format::R32G32B32A32_FLOAT:
		return 4 * 4;
	default:
		throw std::runtime_error("Unknown format size");
	}
}

} // anonymous namespace

FlexibleInputLayoutDescription::PositionElement::PositionElement(size_t index, Format format) :
	index_(index),
	format_(format)
{
}

void FlexibleInputLayoutDescription::PositionElement::toElementDesc(D3D11_INPUT_ELEMENT_DESC* desc) const {
	std::memset(desc, 0, sizeof(*desc));

	desc->SemanticName = "POSITION";
	desc->SemanticIndex = index_;
	desc->Format = static_cast<DXGI_FORMAT>(format_);
	desc->InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	desc->AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
}

size_t FlexibleInputLayoutDescription::PositionElement::size() const {
	return formatSize(format_);
}

void FlexibleInputLayoutDescription::PositionElement::make(
	const VertexInterface& vertex,
	void* buffer
	) const {
	if (format_ != Format::R32G32B32A32_FLOAT) {
		throw std::runtime_error("PositionElement only supports rgba32 format");
	}

	float* target = reinterpret_cast<float*>(buffer);
	math::Vector3d position = vertex.position();

	target[0] = position.x();
	target[1] = position.y();
	target[2] = position.z();
	target[3] = 0.0f;
}

system::COMWrapper<ID3D11InputLayout> FlexibleInputLayoutDescription::makeLayout(
	Device& device,
	void* shaderData,
	size_t shaderSize
	) const {
	std::vector<D3D11_INPUT_ELEMENT_DESC> descs;
	descs.resize(elements_.size());

	for (size_t i = 0; i < elements_.size(); ++i) {
		elements_[i]->toElementDesc(&descs[i]);
	}

	system::COMWrapper<ID3D11InputLayout> layout;
	checkDirectXCall(
		device.d3dDevice()->CreateInputLayout(
			&descs.front(),
			descs.size(),
			shaderData,
			shaderSize,
			&layout.get()
			),
		"Failed to create an input layout"
		);

	return layout;
}

size_t FlexibleInputLayoutDescription::vertexSize() const {
	return std::accumulate(
		elements_.begin(),
		elements_.end(),
		0,
		[](size_t sum, ConstElementSharedPtr element) { return sum + element->size(); }
	);
}

void FlexibleInputLayoutDescription::makeVertex(const VertexInterface& vertex, void* buffer) const {
	std::uint8_t* target = reinterpret_cast<std::uint8_t*>(buffer);
	std::for_each(
		elements_.begin(),
		elements_.end(),
		[&](const ConstElementSharedPtr element) {
			element->make(vertex, target);
			target += element->size();
		}
		);
}

void FlexibleInputLayoutDescription::push(std::shared_ptr<Element> element) {
	elements_.push_back(element);
}
