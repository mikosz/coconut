#include "ShaderFactory.hpp"

#include <vector>
#include <fstream>

#include <coconut-tools/logger.hpp>
#include <coconut-tools/exceptions/RuntimeError.hpp>

#include "coconut/milk/graphics/compile-shader.hpp"
#include "coconut/milk/graphics/ShaderReflection.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;
using namespace coconut::pulp::renderer::shader;

namespace /* anonymous */ {

CT_LOGGER_CATEGORY("COCONUT.PULP.RENDERER.SHADER.SHADER_FACTORY");

std::shared_ptr<UnknownParameter> createParameter(
	ParameterFactory& parameterFactory,
	const milk::graphics::ShaderReflection::Variable& variable
	)
{
	CT_LOG_DEBUG << "Creating parameter for variable " << variable.name;

	using milk::graphics::ShaderReflection;

	//switch (variable.type.klass) {
	//case ShaderReflection::Type::Class::SCALAR:
		return parameterFactory.create(variable.name);
	// TODO
	//}

	//throw "";
}

std::unique_ptr<UnknownShader> createShaderFromCompiledShader(
	milk::graphics::Renderer& graphicsRenderer,
	ParameterFactory& parameterFactory,
	std::vector<std::uint8_t> shaderData,
	milk::graphics::ShaderType shaderType
	)
{
	using milk::graphics::ShaderReflection;

	ShaderReflection reflection(shaderData.data(), shaderData.size());

	UnknownShader::SceneData sceneData;
	UnknownShader::ActorData actorData;
	UnknownShader::MaterialData materialData;
	UnknownShader::Resources resources;

	for (const auto& constantBuffer : reflection.constantBuffers()) {
		for (const auto& variable : constantBuffer.variables) {
			auto parameter = createParameter(parameterFactory, variable);
		}
	}

	switch (shaderType) {
	case milk::graphics::ShaderType::VERTEX:
	{
		milk::graphics::VertexShader vs(graphicsRenderer, shaderData.data(), shaderData.size());
		return std::make_unique<VertexShader>(
			std::move(vs), std::move(sceneData), std::move(actorData), std::move(materialData), std::move(resources));
	}
	case milk::graphics::ShaderType::PIXEL:
	{
		milk::graphics::PixelShader ps(graphicsRenderer, shaderData.data(), shaderData.size());
		return std::make_unique<PixelShader>(
			std::move(ps), std::move(sceneData), std::move(actorData), std::move(materialData), std::move(resources));
	}
	}

	throw coconut_tools::exceptions::RuntimeError("Unexpected shader type: " + toString(shaderType));
}

std::vector<std::uint8_t> readContents(const boost::filesystem::path& path) {
	std::vector<std::uint8_t> contents;
	std::ifstream ifs(path.string().c_str(), std::ios::binary); // TODO: this needs to move to some utility place
	std::ostringstream oss;
	oss << ifs.rdbuf();
	auto s = oss.str();
	contents.reserve(s.length());
	std::copy(s.begin(), s.end(), std::back_inserter(contents));

	return contents;
}

std::unique_ptr<UnknownShader> createShaderFromCompiledShader(
	milk::graphics::Renderer& graphicsRenderer,
	ParameterFactory& parameterFactory,
	const detail::ShaderCreator::CompiledShaderInfo& compiledShaderInfo
	)
{
	return createShaderFromCompiledShader(
		graphicsRenderer,
		parameterFactory,
		readContents(compiledShaderInfo.compiledShaderPath),
		compiledShaderInfo.shaderType
		);
}

std::unique_ptr<UnknownShader> createShaderFromShaderCode(
	milk::graphics::Renderer& graphicsRenderer,
	ParameterFactory& parameterFactory,
	const detail::ShaderCreator::ShaderCodeInfo& shaderCodeInfo
	)
{
	CT_LOG_INFO << "Compiling shader at " << shaderCodeInfo.shaderCodePath << "::" << shaderCodeInfo.entrypoint;

	auto shaderData = milk::graphics::compileShader(
		readContents(shaderCodeInfo.shaderCodePath), shaderCodeInfo.entrypoint);

	return createShaderFromCompiledShader(
		graphicsRenderer,
		parameterFactory,
		shaderData,
		shaderCodeInfo.shaderType
		);
}

} // anonymous namespace

std::unique_ptr<UnknownShader> detail::ShaderCreator::doCreate(
	const std::string& id, milk::graphics::Renderer& graphicsRenderer)
{
	CT_LOG_INFO << "Creating shader: \"" << id << "\"";

	if (shaderCodeInfos_.count(id) != 0) {
		CT_LOG_DEBUG << "Found \"" << id << "\" registered as shader code";
		return createShaderFromShaderCode(graphicsRenderer, parameterFactory_, shaderCodeInfos_[id]);
	} else if (compiledShaderInfos_.count(id) != 0) {
		CT_LOG_DEBUG << "Found \"" << id << "\" registered as a compiled shader";
		return createShaderFromCompiledShader(graphicsRenderer, parameterFactory_, compiledShaderInfos_[id]);
	} else {
		throw coconut_tools::factory::error_policy::NoSuchType<std::string>(id);
	}
}

void detail::ShaderCreator::registerShaderCode(std::string id, const ShaderCodeInfo& shaderCodeInfo) {
	CT_LOG_INFO << "Registering shader code \"" << id << "\" (" << shaderCodeInfo.shaderType
		<< ") at " << shaderCodeInfo.shaderCodePath << "::" << shaderCodeInfo.entrypoint;

	if (shaderCodeInfos_.count(id) != 0 || compiledShaderInfos_.count(id) != 0) {
		throw coconut_tools::factory::error_policy::CreatorAlreadyRegistered<std::string>(id);
	}

	shaderCodeInfos_.emplace(std::move(id), std::move(shaderCodeInfo));
}

void detail::ShaderCreator::registerCompiledShader(std::string id, const CompiledShaderInfo& compiledShaderInfo) {
	CT_LOG_INFO << "Registering compiled shader \"" << id << "\" (" << compiledShaderInfo.shaderType
		<< ") at " << compiledShaderInfo.compiledShaderPath.string();

	if (shaderCodeInfos_.count(id) != 0 || compiledShaderInfos_.count(id) != 0) {
		throw coconut_tools::factory::error_policy::CreatorAlreadyRegistered<std::string>(id);
	}

	compiledShaderInfos_.emplace(std::move(id), std::move(compiledShaderInfo));
}
