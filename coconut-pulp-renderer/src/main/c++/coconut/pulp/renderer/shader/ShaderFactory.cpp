#include "ShaderFactory.hpp"

#include <vector>
#include <fstream>
#include <algorithm>

#include <coconut-tools/logger.hpp>
#include <coconut-tools/exceptions/RuntimeError.hpp>

#include "coconut/milk/graphics/compile-shader.hpp"
#include "coconut/milk/graphics/ShaderReflection.hpp"
#include "StructuredParameter.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;
using namespace coconut::pulp::renderer::shader;

namespace /* anonymous */ {

CT_LOGGER_CATEGORY("COCONUT.PULP.RENDERER.SHADER.SHADER_FACTORY");

using milk::graphics::ShaderReflection;

std::shared_ptr<UnknownParameter> createParameter(
	ParameterFactory& parameterFactory,
	const std::string& name,
	const ShaderReflection::Type& type,
	const std::string& parentTypeName
	)
{
	CT_LOG_DEBUG << "Creating parameter for variable " << name;

	std::vector<int> indices = { -1 };

	if (type.elements > 0) {
		indices.resize(type.elements);
		auto index = 0;
		std::for_each(indices.begin(), indices.end(), [&index](int& element) { element = index++; });
	}

	if (type == ShaderReflection::Type::Class::STRUCT) {
		StructuredParameter<???>::Subparameters structureParameters;

		for (const auto& member : type.members) {
			std::string memberName;
			ShaderReflection::Type memberType;
			std::tie(memberName, memberType) = member;

			structureParameters.emplace_back(
				createParameter(parameterFactory, memberName, memberType, parentTypeName + "::" + type.name)
				);
		}
	}

	ParameterFactoryInstanceDetails instanceDetails(name);

	for (size_t arrayElementIndex = 0; arrayElementIndex < arrayElements; ++arrayElementIndex) {
		instanceDetails.id = ;
		instanceDetails.arrayedElementIndex = arrayElementIndex;

		switch (variable.type.klass) {
		case ShaderReflection::Type::Class::SCALAR:
			return parameterFactory.create(instanceDetails);
		}
	}

	throw "";
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
			auto parameter = createParameter(parameterFactory, variable.name, variable.type, "");
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
