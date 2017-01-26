#include "FlexibleInputLayoutDescription.hpp"

#include <algorithm>
#include <numeric>
#include <unordered_map>
#include <sstream>

#include <boost/range/adaptor/filtered.hpp>

#include <coconut-tools/exceptions/LogicError.hpp>

#include "DirectXError.hpp"
#include "Renderer.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::graphics;

using namespace std::string_literals;

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

void makeElement(
	const FlexibleInputLayoutDescription::Element& element,
	const VertexInterface& vertex,
	void* buffer
	) {
	switch (element.type) {
	case FlexibleInputLayoutDescription::ElementType::POSITION:
		{
			if (element.format != FlexibleInputLayoutDescription::Format::R32G32B32A32_FLOAT) {
				throw std::runtime_error("PositionElement only supports rgba32 float format"); // TODO: exc
			}

			const auto target = reinterpret_cast<float*>(buffer);
			const auto position = vertex.position();

			target[0] = position.x();
			target[1] = position.y();
			target[2] = position.z();
			target[3] = 0.0f; // XXX: 1.0f?

			break;
		}
	case FlexibleInputLayoutDescription::ElementType::NORMAL:
		{
			if (element.format != FlexibleInputLayoutDescription::Format::R32G32B32_FLOAT) {
				throw std::runtime_error("NormalElement only supports rgb32 float format");
			}

			const auto target = reinterpret_cast<float*>(buffer);
			const auto normal = vertex.normal();

			target[0] = normal.x();
			target[1] = normal.y();
			target[2] = normal.z();

			break;
		}
	case FlexibleInputLayoutDescription::ElementType::TEXTURE_COORDINATES:
		{
			if (element.format != FlexibleInputLayoutDescription::Format::R32G32_FLOAT) {
				throw std::runtime_error("TextureCoordinatesElement only supports rg32 float format");
			}

			const auto target = reinterpret_cast<float*>(buffer);
			const auto position = vertex.textureCoordinate(); // TODO: only 2d, so thats an issue

			target[0] = position.x();
			target[1] = position.y();

			break;
		}
	case FlexibleInputLayoutDescription::ElementType::INSTANCE_ID:
		break;
	default:
		assert(false && "input layout element implementation missing");
	}
}

size_t inputSlotIndex(InputLayoutDescription::SlotType inputSlot) {
	switch (inputSlot) {
	case InputLayoutDescription::SlotType::PER_VERTEX_DATA:
		return 0u;
	case InputLayoutDescription::SlotType::PER_INSTANCE_DATA:
		return 1u;
	default:
		throw coconut_tools::exceptions::LogicError("input layout slot implementation missing");
	}
}

} // anonymous namespace

const std::string FlexibleInputLayoutDescription::POSITION_SEMANTIC = "POSITION"s;
const std::string FlexibleInputLayoutDescription::TEXTURE_ELEMENT_SEMANTIC = "TEXCOORD"s;
const std::string FlexibleInputLayoutDescription::NORMAL_SEMANTIC = "NORMAL"s;
const std::string FlexibleInputLayoutDescription::INSTANCE_ID_SEMANTIC = "SV_InstanceID"s;

system::COMWrapper<ID3D11InputLayout> FlexibleInputLayoutDescription::makeLayout(
	Renderer& renderer,
	const void* shaderData,
	size_t shaderSize
	) const
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> descs;
	descs.reserve(elements_.size());

	for (const auto& element : elements_ | boost::adaptors::filtered([](const auto& element) {
				return element.type != ElementType::INSTANCE_ID;
			})) {
		descs.emplace_back();
		auto& desc = descs.back();

		std::memset(&desc, 0, sizeof(desc));

		desc.SemanticName = toString(element.type).c_str();
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
			const_cast<void*>(shaderData),
			shaderSize,
			&layout.get()
			),
		"Failed to create an input layout"
		);

	return layout;
}

size_t FlexibleInputLayoutDescription::vertexSize(SlotType slotType) const {
	return std::accumulate(
		elements_.begin(),
		elements_.end(),
		static_cast<size_t>(0),
		[](size_t sum, const Element& element) { return sum + formatSize(element.format); }
	);
}

void FlexibleInputLayoutDescription::makeVertex(
	const VertexInterface& vertex, void* buffer, SlotType slotType) const
{
	std::uint8_t* target = reinterpret_cast<std::uint8_t*>(buffer);
	for (const auto& element : elements_ | boost::adaptors::filtered([slotType](const auto& element) {
			return (element.inputSlotType == slotType) && (element.type != ElementType::INSTANCE_ID);
		}))
	{
		makeElement(element, vertex, target);
		target += formatSize(element.format);
	}
}

void FlexibleInputLayoutDescription::push(Element element) {
	elements_.emplace_back(std::move(element));
}

std::vector<std::uint8_t> FlexibleInputLayoutDescription::createDummyVertexShader() const {
	const auto* const prefix = "struct VIn {\n";
	const auto* const suffix = 
		"};\n"
		"\n"
		"SV_POSITION main(VIn in) { return (0.0f).xxxx; }\n"
		;

	std::ostringstream shaderTextStream;
	shaderTextStream << prefix;

	for (const auto& element : elements_) {
		shaderTextStream
			<< "\t"
			<< formatHLSLType(element.format)
			<< toString(element.type) << "_" << element.semanticIndex
			<< " : "
			<< toString(element.type)
			<< ";\n";
	}

	std::vector<std::uint8_t> shaderData;
	const auto shaderText = shaderTextStream.str();
	shaderData.reserve(shaderText.size());
	std::copy(shaderText.begin(), shaderText.end(), std::back_inserter(shaderData));

	return compileShader(shaderData, "main");
}
