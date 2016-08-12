#include "ShaderReflection.hpp"

#include <d3d11shader.h>
#include <d3dcompiler.h>

#include "coconut/milk/system/cleanup-windows-macros.hpp"
#include <coconut-tools/logger.hpp>

#include "DirectXError.hpp"

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::graphics;

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

		ShaderReflection::InputParameterInfo info;
		fromString(info.semantic, desc.SemanticName);
		info.semanticIndex = desc.SemanticIndex;

		CT_LOG_DEBUG << "Shader input parameter " << i << ": " << toString(info.semantic) << "#" << info.semanticIndex;

		inputParameters.emplace_back(std::move(info));
	}

	return inputParameters;
}

ShaderReflection::ConstantBufferInfo::Variable buildConstantBufferVariable(
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

	ShaderReflection::ConstantBufferInfo::Variable variable;
	variable.name = desc.Name;
	variable.size = desc.Size;
	variable.offset = desc.StartOffset;

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

		info.size = desc.Size;
		for (size_t variableIdx = 0; variableIdx < desc.Variables; ++variableIdx) {
			info.variables.emplace_back(buildConstantBufferVariable(*constantBufferInfo, variableIdx));
		}

		constantBuffers.emplace_back(std::move(info));
	}

	return constantBuffers;
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
}
