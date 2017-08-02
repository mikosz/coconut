#include "ShaderReflection.hpp"

#include <algorithm>

#include <d3d11shader.h>
#include <d3dcompiler.h>
#include "coconut/milk/system/cleanup-windows-macros.hpp"

#include <coconut-tools/logger.hpp>

#include "coconut/milk/utils/bits.hpp"
#include "coconut/milk/utils/integralValue.hpp"

#include "DirectXError.hpp"

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::graphics;

using namespace std::string_literals;

namespace /* anonymous */ {

CT_LOGGER_CATEGORY("COCONUT.MILK.GRAPHICS.SHADER_REFLECTION");

ShaderReflection::InputParameterInfos buildInputParameterInfos(
	system::COMWrapper<ID3D11ShaderReflection> reflectionData, const D3D11_SHADER_DESC& shaderDescription
	) {
	ShaderReflection::InputParameterInfos inputParameters;

	for (size_t i = 0; i < shaderDescription.InputParameters; ++i) {
		D3D11_SIGNATURE_PARAMETER_DESC desc;
		std::memset(&desc, 0, sizeof(desc));
		checkDirectXCall(
			reflectionData->GetInputParameterDesc(static_cast<UINT>(i), &desc),
			"Failed to get input parameter description"
			);

		if (
			desc.SemanticName == "SV_InstanceID"s ||
			desc.SemanticName == "SV_VertexID"s
			) { // TODO: extract literals
			continue;
		}

		ShaderReflection::InputParameterInfo info;
		info.semantic = desc.SemanticName;

		info.semanticIndex = desc.SemanticIndex;
		fromIntegral(info.dataType, milk::utils::integralValue(desc.ComponentType));
		info.elements = milk::utils::numberOfBitsOn(desc.Mask);

		CT_LOG_DEBUG << "Shader input parameter " << i << ": " << info.semantic << "#" << info.semanticIndex;

		inputParameters.emplace_back(std::move(info));
	}

	return inputParameters;
}

ShaderReflection::Type buildTypeInfo(ID3D11ShaderReflectionType& typeInfo, size_t size) {
	auto desc = D3D11_SHADER_TYPE_DESC();
	checkDirectXCall(typeInfo.GetDesc(&desc), "Failed to get variable type desc");

	CT_LOG_DEBUG << "Shader variable type: " << (desc.Name ? desc.Name : "<NULL>");

	ShaderReflection::Type type;
	type.name = (desc.Name ? desc.Name : "");
	fromIntegral(type.klass, milk::utils::integralValue(desc.Class));
	fromIntegral(type.scalarType, milk::utils::integralValue(desc.Type));
	type.columns = desc.Columns;
	type.rows = desc.Rows;
	type.offset = desc.Offset;
	type.elements = desc.Elements;

	const auto elementSize = size / std::max<size_t>(type.elements, 1);
	type.elementOffset = milk::utils::roundUpToMultipleOf(elementSize, 16);

	for (UINT memberIdx = 0; memberIdx < desc.Members; ++memberIdx) {
		auto* memberType = typeInfo.GetMemberTypeByIndex(memberIdx);
		auto memberTypeDesc = D3D11_SHADER_TYPE_DESC();
		checkDirectXCall(typeInfo.GetDesc(&memberTypeDesc), "Failed to get member type desc");

		size_t memberSize; // hehehe

		if (memberIdx < desc.Members - 1) {
			auto nextMemberTypeDesc = D3D11_SHADER_TYPE_DESC();
			checkDirectXCall(typeInfo.GetDesc(&nextMemberTypeDesc), "Failed to get member type desc");
			memberSize = nextMemberTypeDesc.Offset - memberTypeDesc.Offset;
		} else {
			memberSize = desc.Offset + size - memberTypeDesc.Offset;
		}

		type.members.emplace_back(
			typeInfo.GetMemberTypeName(memberIdx),
			buildTypeInfo(*memberType, memberSize)
			);
	}

	return type;
}

ShaderReflection::Variable buildVariableInfo(
	ID3D11ShaderReflectionConstantBuffer& constantBufferInfo, size_t variableIdx
	) {
	auto* variableInfo = constantBufferInfo.GetVariableByIndex(static_cast<UINT>(variableIdx));
	D3D11_SHADER_VARIABLE_DESC desc;
	std::memset(&desc, 0, sizeof(desc));
	checkDirectXCall(
		variableInfo->GetDesc(&desc),
		"Failed to get variable info description"
		);

	CT_LOG_DEBUG << "Shader constant buffer variable " << variableIdx << ": " << desc.Name;

	ShaderReflection::Variable variable;
	variable.name = desc.Name;
	variable.size = desc.Size;
	variable.offset = desc.StartOffset;
	variable.type = buildTypeInfo(*variableInfo->GetType(), variable.size);

	return variable;
}

ShaderReflection::ConstantBufferInfos buildConstantBufferInfos(
	system::COMWrapper<ID3D11ShaderReflection> reflectionData, const D3D11_SHADER_DESC& shaderDescription
	) {
	ShaderReflection::ConstantBufferInfos constantBuffers;

	for (size_t bufferIdx = 0; bufferIdx < shaderDescription.ConstantBuffers; ++bufferIdx) {
		auto* constantBufferInfo = reflectionData->GetConstantBufferByIndex(static_cast<UINT>(bufferIdx));
		D3D11_SHADER_BUFFER_DESC desc;
		std::memset(&desc, 0, sizeof(desc));
		checkDirectXCall(
			constantBufferInfo->GetDesc(&desc),
			"Failed to get constant buffer description"
			);

		ShaderReflection::ConstantBufferInfo info;

		CT_LOG_DEBUG << "Shader constant buffer " << bufferIdx << ": " << desc.Name;

		info.name = desc.Name;
		info.size = desc.Size;
		info.slot = bufferIdx; // TODO: verify can't get manually set slot
		for (size_t variableIdx = 0; variableIdx < desc.Variables; ++variableIdx) {
			info.variables.emplace_back(buildVariableInfo(*constantBufferInfo, variableIdx));
		}

		constantBuffers.emplace_back(std::move(info));
	}

	return constantBuffers;
}

ShaderReflection::ResourceInfos buildResourceInfos(
	system::COMWrapper<ID3D11ShaderReflection> reflectionData, const D3D11_SHADER_DESC& shaderDescription
	) {
	ShaderReflection::ResourceInfos	 resources;

	for (size_t resourceIdx = 0; resourceIdx < shaderDescription.BoundResources; ++resourceIdx) {
		D3D11_SHADER_INPUT_BIND_DESC resourceDesc;
		checkDirectXCall(
			reflectionData->GetResourceBindingDesc(static_cast<UINT>(resourceIdx), &resourceDesc),
			"Failed to get resource description"
			);

		ShaderReflection::ResourceInfo info;
		
		if (resourceDesc.Type != D3D_SIT_SAMPLER && resourceDesc.Type != D3D_SIT_TEXTURE) {
			continue;
		}

		fromIntegral(info.type, milk::utils::integralValue(resourceDesc.Type));
		info.name = resourceDesc.Name;
		info.slot = resourceDesc.BindPoint;
		assert(resourceDesc.BindCount == 1);
		fromIntegral(info.dimension, milk::utils::integralValue(resourceDesc.Dimension));

		resources.emplace_back(std::move(info));
	}

	return resources;
}

} // anonymous namespace

ShaderReflection::ShaderReflection(const void* shaderData, size_t shaderSize) {
	system::COMWrapper<ID3D11ShaderReflection> reflectionData;
	checkDirectXCall(
		D3DReflect(shaderData, shaderSize, IID_ID3D11ShaderReflection, reinterpret_cast<void**>(&reflectionData.get())),
		"Failed to get reflection data for shader"
		);

	D3D11_SHADER_DESC shaderDescription;
	std::memset(&shaderDescription, 0, sizeof(shaderDescription));
	checkDirectXCall(
		reflectionData->GetDesc(&shaderDescription),
		"Failed to get shader description"
		);

	inputParameters_ = buildInputParameterInfos(reflectionData, shaderDescription);
	constantBuffers_ = buildConstantBufferInfos(reflectionData, shaderDescription);
	resources_ = buildResourceInfos(reflectionData, shaderDescription);
}
