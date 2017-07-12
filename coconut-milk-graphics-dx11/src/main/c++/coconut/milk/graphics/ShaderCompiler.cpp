#include "ShaderCompiler.hpp"

#include <algorithm>
#include <cstdlib>

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
	case ShaderType::GEOMETRY:
		return "gs_5_0"s;
	case ShaderType::HULL:
		return "hs_5_0"s;
	case ShaderType::DOMAIN:
		return "ds_5_0"s;
	case ShaderType::PIXEL:
		return "ps_5_0"s;
	default:
		assert(!"Unsupported shader type"); // TODO: should throw
		return ""s;
	}
}

class Includer : public ID3DInclude {
public:

	Includer(ShaderCompiler::IncludeHandler handler) :
		handler_(std::move(handler))
	{
	}

	HRESULT Open(
		D3D_INCLUDE_TYPE includeType,
		LPCSTR fileName,
		LPCVOID parentData,
		LPCVOID* data,
		UINT* bytes
		) override
	{
		data_.emplace_back(handler_(fileName));
		*data = data_.back()->data();
		*bytes = data_.back()->size();
		return S_OK;
	}

	HRESULT Close(LPCVOID data) override {
		return S_OK;
	}

private:

	ShaderCompiler::IncludeHandler handler_;

	std::vector<std::shared_ptr<ShaderCompiler::ShaderData>> data_;

};

} // anonymous namespace

ShaderCompiler::ShaderData ShaderCompiler::compile(
    const std::vector<std::uint8_t>& code,
    const std::string& name,
	const std::string& entrypoint,
    ShaderType type,
    IncludeHandler includeHandler,
    CompilerFlags instanceFlags
    ) const
{
	auto includer = Includer(includeHandler);

	system::COMWrapper<ID3DBlob> codeBlob;
	system::COMWrapper<ID3DBlob> errors;
	auto result = D3DCompile(
		code.data(),
		code.size(),
		nullptr, // TODO: shader name
		nullptr,
		&includer,
		entrypoint.c_str(),
		featureLevel(type).c_str(),
		(globalCompilerFlags_ | instanceFlags).integralValue(),
		0,
		&codeBlob.get(),
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

		CT_LOG(logLevel) << prefix << ": " << reinterpret_cast<const char*>(errors->GetBufferPointer());
	}

	if (FAILED(result)) {
		throw DirectXError(result, "Failed to compile a shader");
	}

	std::vector<std::uint8_t> data;
	data.reserve(codeBlob->GetBufferSize());
	std::copy(
		reinterpret_cast<const std::uint8_t*>(codeBlob->GetBufferPointer()),
		reinterpret_cast<const std::uint8_t*>(codeBlob->GetBufferPointer()) + codeBlob->GetBufferSize(),
		std::back_inserter(data)
		);

	return data;
}
