#include "compile-shader.hpp"

#include <algorithm>
#include <cstdlib>

#include <d3dcompiler.h>
#include "coconut/milk/system/cleanup-windows-macros.hpp"

#include "DirectXError.hpp"

#include <coconut-tools/logger.hpp>

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::graphics;

using namespace std::string_literals;

namespace /* anonymous */ {

CT_LOGGER_CATEGORY("COCONUT.MILK.GRAPHICS.COMPILE_SHADER");

std::string featureLevel(ShaderType shaderType) {
	switch (shaderType) {
	case ShaderType::VERTEX:
		return "vs_5_0"s;
	case ShaderType::PIXEL:
		return "ps_5_0"s;
	default:
		assert(!"Unsupported shader type"); // TODO: should throw
		return ""s;
	}
}

} // anonymous namespace

std::vector<std::uint8_t> coconut::milk::graphics::compileShader(
	const std::vector<std::uint8_t>& shaderData,
	const std::string& entrypoint,
	ShaderType shaderType
	)
{
	system::COMWrapper<ID3DBlob> code;
	system::COMWrapper<ID3DBlob> errors;
	auto result = D3DCompile(
		shaderData.data(),
		shaderData.size(),
		nullptr,
		nullptr,
		nullptr,
		entrypoint.c_str(),
		featureLevel(shaderType).c_str(),
		0,
		0,
		&code.get(),
		&errors.get()
		);

	if (errors) {
		coconut_tools::logger::Level logLevel;
		std::string prefix;

		if (FAILED(result)) {
			prefix = "HLSL compiler error";
			logLevel = coconut_tools::logger::Level::CRITICAL;
		} else {
			prefix = "HLSL compiler warning";
			logLevel = coconut_tools::logger::Level::WARNING;
		}

		const auto* wideErrors = reinterpret_cast<const wchar_t*>(errors->GetBufferPointer());
		auto errors = std::vector<char>((std::wcslen(wideErrors) * MB_CUR_MAX) + 1, '\0');
		auto res = std::wcstombs(errors.data(), wideErrors, errors.size());
		CT_LOG(logLevel) << prefix << ": " << errors;
	}

	if (FAILED(result)) {
		throw DirectXError(result, "Failed to compile a shader");
	}

	std::vector<std::uint8_t> data;
	data.reserve(code->GetBufferSize());
	std::copy(
		reinterpret_cast<const std::uint8_t*>(code->GetBufferPointer()),
		reinterpret_cast<const std::uint8_t*>(code->GetBufferPointer()) + code->GetBufferSize(),
		std::back_inserter(data)
		);

	return data;
}
