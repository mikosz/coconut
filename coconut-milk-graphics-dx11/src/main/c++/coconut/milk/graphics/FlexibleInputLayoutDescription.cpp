#include "FlexibleInputLayoutDescription.hpp"

#include <algorithm>
#include <numeric>

#include "DirectXError.hpp"
#include "Renderer.hpp"

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
	desc->SemanticIndex = static_cast<UINT>(index_);
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
	target[3] = 0.0f; // XXX: 1.0f?
}

FlexibleInputLayoutDescription::TextureCoordinatesElement::TextureCoordinatesElement(size_t index, Format format) :
	index_(index),
	format_(format)
{
}

void FlexibleInputLayoutDescription::TextureCoordinatesElement::toElementDesc(D3D11_INPUT_ELEMENT_DESC* desc) const {
	std::memset(desc, 0, sizeof(*desc));

	desc->SemanticName = "TEXCOORD";
	desc->SemanticIndex = static_cast<UINT>(index_);
	desc->Format = static_cast<DXGI_FORMAT>(format_);
	desc->InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	desc->AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
}

size_t FlexibleInputLayoutDescription::TextureCoordinatesElement::size() const {
	return formatSize(format_);
}

void FlexibleInputLayoutDescription::TextureCoordinatesElement::make(
	const VertexInterface& vertex,
	void* buffer
	) const {
	if (format_ != Format::R32G32_FLOAT) {
		throw std::runtime_error("PositionElement only supports rg32 format");
	}

	float* target = reinterpret_cast<float*>(buffer);
	auto&& position = vertex.textureCoordinate(); // TODO: only 2d, so thats an issue

	target[0] = position.x();
	target[1] = position.y();
}

FlexibleInputLayoutDescription::NormalElement::NormalElement(size_t index, Format format) :
	index_(index),
	format_(format)
{
}

void FlexibleInputLayoutDescription::NormalElement::toElementDesc(D3D11_INPUT_ELEMENT_DESC* desc) const {
	std::memset(desc, 0, sizeof(*desc));

	desc->SemanticName = "NORMAL";
	desc->SemanticIndex = static_cast<UINT>(index_);
	desc->Format = static_cast<DXGI_FORMAT>(format_);
	desc->InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	desc->AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
}

size_t FlexibleInputLayoutDescription::NormalElement::size() const {
	return formatSize(format_);
}

void FlexibleInputLayoutDescription::NormalElement::make(
	const VertexInterface& vertex,
	void* buffer
	) const {
	if (format_ != Format::R32G32B32_FLOAT) {
		throw std::runtime_error("PositionElement only supports rgb32 format");
	}

	float* target = reinterpret_cast<float*>(buffer);
	auto&& normal = vertex.normal();

	target[0] = normal.x();
	target[1] = normal.y();
	target[2] = normal.z();
}

system::COMWrapper<ID3D11InputLayout> FlexibleInputLayoutDescription::makeLayout(
	Renderer& renderer,
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
		renderer.internalDevice().CreateInputLayout(
			&descs.front(),
			static_cast<UINT>(descs.size()),
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
		static_cast<size_t>(0),
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
