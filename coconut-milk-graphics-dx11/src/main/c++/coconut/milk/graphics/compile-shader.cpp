#include "compile-shader.hpp"

#include <algorithm>

#include <d3dcompiler.h>
#include "coconut/milk/system/cleanup-windows-macros.hpp"

#include "DirectXError.hpp"

#include <coconut-tools/logger.hpp>

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::graphics;

namespace /* anonymous */ {

CT_LOGGER_CATEGORY("COCONUT.MILK.GRAPHICS.COMPILE_SHADER");

} // anonymous namespace

std::vector<std::uint8_t> coconut::milk::graphics::compileShader(
	const std::vector<std::uint8_t>& shaderData,
	const std::string& entrypoint
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
		nullptr,
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

		CT_LOG(logLevel) << prefix << ": " << reinterpret_cast<const wchar_t*>(errors->GetBufferPointer());
	}

	if (FAILED(result)) {
		throw DirectXError(result, "Failed to compile a shader");
	}

	std::vector<std::uint8_t> data;
	data.reserve(code->GetBufferSize());
	std::copy(
		reinterpret_cast<const std::uint8_t*>(code->GetBufferPointer()),
		reinterpret_cast<const std::uint8_t*>(code->GetBufferPointer()) + data.size(),
		std::back_inserter(data)
		);

	return data;
}
