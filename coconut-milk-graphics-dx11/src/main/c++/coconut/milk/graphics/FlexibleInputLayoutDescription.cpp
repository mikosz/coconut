#include "FlexibleInputLayoutDescription.hpp"

#include <algorithm>
#include <numeric>
#include <unordered_map>
#include <sstream>

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

const char* const formatHLSLType(FlexibleInputLayoutDescription::Format format) {
	switch (format) {
	case FlexibleInputLayoutDescription::Format::R32G32_FLOAT:
		return "float2";
	case FlexibleInputLayoutDescription::Format::R32G32B32_FLOAT:
		return "float3";
	case FlexibleInputLayoutDescription::Format::R32G32B32A32_FLOAT:
		return "float4";
	default:
		throw std::runtime_error("Unknown format size");
	}
}

} // anonymous namespace

const std::string FlexibleInputLayoutDescription::PositionElement::HLSL_SEMANTIC_ = "POSITION";
const std::string FlexibleInputLayoutDescription::TextureCoordinatesElement::HLSL_SEMANTIC_ = "TEXCOORD";
const std::string FlexibleInputLayoutDescription::NormalElement::HLSL_SEMANTIC_ = "NORMAL";

FlexibleInputLayoutDescription::PositionElement::PositionElement(size_t index, Format format) :
	index_(index),
	format_(format)
{
}

void FlexibleInputLayoutDescription::PositionElement::toElementDesc(D3D11_INPUT_ELEMENT_DESC* desc) const {
	std::memset(desc, 0, sizeof(*desc));

	desc->SemanticName = HLSL_SEMANTIC_.c_str();
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

	desc->SemanticName = HLSL_SEMANTIC_.c_str();
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

	desc->SemanticName = HLSL_SEMANTIC_.c_str();
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
	const void* shaderData,
	size_t shaderSize
	) const
{
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
			const_cast<void*>(shaderData),
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

void FlexibleInputLayoutDescription::push(std::shared_ptr<Element> element) { // TODO: why shared?
	elements_.push_back(element);
}

std::vector<std::uint8_t> FlexibleInputLayoutDescription::createDummyVertexShader() const {
	const auto* const prefix = "struct VIn {\n";
	const auto* const suffix = 
		"};\n"
		"\n"
		"SV_POSITION main(VIn in) { return float4(0.0f, 0.0f, 0.0f, 0.0f); }\n"
		;

	std::ostringstream shaderTextStream;
	shaderTextStream << prefix;

	for (const auto& element : elements_) {
		shaderTextStream
			<< "\t"
			<< formatHLSLType(element->format())
			<< element->hlslSemantic() << "_" << element->index()
			<< " : "
			<< element->hlslSemantic()
			<< ";\n";
	}

	std::vector<std::uint8_t> shaderData;
	const auto shaderText = shaderTextStream.str();
	shaderData.reserve(shaderText.size());
	std::copy(shaderText.begin(), shaderText.end(), std::back_inserter(shaderData));

	return compileShader(shaderData, "main");
}
