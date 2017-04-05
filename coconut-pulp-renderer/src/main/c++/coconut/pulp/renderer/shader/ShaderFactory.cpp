#include "ShaderFactory.hpp"

#include <vector>
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

std::shared_ptr<Parameter> createParameter(
	ParameterFactory& parameterFactory,
	const std::string& name,
	const ShaderReflection::Type& type,
	size_t offset,
	const std::string& parentType = std::string()
	)
{
	CT_LOG_DEBUG << "Creating parameter for variable " << name;

	std::shared_ptr<Parameter> result;

	ParameterFactoryInstanceDetails instanceDetails(name);

	instanceDetails.padding = offset + type.offset;
	instanceDetails.arraySize = type.elements;
	instanceDetails.arrayElementOffset = type.elementOffset; // TODO: nope
	instanceDetails.parentType = parentType;

	auto parameter = parameterFactory.create(instanceDetails);

	if (parameter->outputType() == Parameter::OperandType::OBJECT) {
		if (type.klass != ShaderReflection::Type::Class::STRUCT) {
#pragma message("!!! TODO: exception + want a common superclass for shader factory exceptions")
			throw "";
		}

		auto& structuredParameter = dynamic_cast<StructuredParameter&>(*parameter);

		for (const auto& member : type.members) {
			std::string memberName;
			ShaderReflection::Type memberType;
			std::tie(memberName, memberType) = member;

			structuredParameter.addSubparameter(
				createParameter(parameterFactory, memberName, memberType, type.offset, type.name)
				);
		}
	} else if (parameter->outputType() == Parameter::OperandType::MATRIX) {
		if (type.klass == ShaderReflection::Type::Class::MATRIX_COLUMN_MAJOR) {
			ParameterFactoryInstanceDetails instanceDetails("transpose"); // TODO: not elegant really
			parameter->setNext(parameterFactory.create(instanceDetails));
			// TODO: remove last if last is transpose
		}
	}

#pragma message("!!! TODO: verify size and output type before returning") // TODO

	return parameter;
}

std::shared_ptr<Resource> createResource(
	ResourceFactory& resourceFactory,
	const ShaderReflection::ResourceInfo& resourceInfo,
	milk::graphics::ShaderType shaderType
	)
{
	switch (resourceInfo.type) {
		case ShaderReflection::ResourceInfo::Type::TEXTURE: // fallthrough
		case ShaderReflection::ResourceInfo::Type::SAMPLER:
			return resourceFactory.create(resourceInfo.name, shaderType, resourceInfo.slot);
		default:
			throw "";
#pragma message("!!! TODO: exception") // TODO
	}
}

std::unique_ptr<UnknownShader> createShaderFromCompiledShader(
	milk::graphics::Renderer& graphicsRenderer,
	ParameterFactory& parameterFactory,
	ResourceFactory& resourceFactory,
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
		std::vector<ParameterSharedPtr> parameters;
		parameters.reserve(constantBuffer.variables.size());

		for (const auto& variable : constantBuffer.variables) {
			auto parameter = createParameter(parameterFactory, variable.name, variable.type, variable.offset);

			if (!parameters.empty() && parameters.back()->inputType() != parameter->inputType()) {
#pragma message("!!! TODO: exception") // TODO
				throw "";
			}

			parameters.emplace_back(parameter);
		}

		if (!parameters.empty()) {
			switch (parameters.back()->inputType()) {
			case Parameter::OperandType::SCENE:
				sceneData.emplace_back(std::make_unique<ConstantBuffer<Scene>>(
					graphicsRenderer, shaderType, constantBuffer.size, constantBuffer.slot, std::move(parameters)
					));
				break;
			case Parameter::OperandType::ACTOR:
				actorData.emplace_back(std::make_unique<ConstantBuffer<std::vector<const Actor*>>>(
					graphicsRenderer, shaderType, constantBuffer.size, constantBuffer.slot, std::move(parameters)
					));
				break;
			case Parameter::OperandType::MATERIAL:
				materialData.emplace_back(std::make_unique<ConstantBuffer<Material>>(
					graphicsRenderer, shaderType, constantBuffer.size, constantBuffer.slot, std::move(parameters)
					));
				break;
			default:
				throw "";
#pragma message("!!! TODO: exception") // TODO
			}
		}
	}

	for (const auto& resource : reflection.resources()) {
		resources.emplace_back(createResource(resourceFactory, resource, shaderType));
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

std::unique_ptr<UnknownShader> createShaderFromCompiledShader(
	milk::graphics::Renderer& graphicsRenderer,
	const milk::FilesystemContext& filesystemContext,
	ParameterFactory& parameterFactory,
	ResourceFactory& resourceFactory,
	const detail::ShaderCreator::CompiledShaderInfo& compiledShaderInfo
	)
{
	return createShaderFromCompiledShader(
		graphicsRenderer,
		parameterFactory,
		resourceFactory,
		*filesystemContext.load(compiledShaderInfo.compiledShaderPath),
		compiledShaderInfo.shaderType
		);
}

std::unique_ptr<UnknownShader> createShaderFromShaderCode(
	milk::graphics::Renderer& graphicsRenderer,
	const milk::FilesystemContext& filesystemContext,
	ParameterFactory& parameterFactory,
	ResourceFactory& resourceFactory,
	const detail::ShaderCreator::ShaderCodeInfo& shaderCodeInfo
	)
{
	CT_LOG_INFO << "Compiling shader at " << shaderCodeInfo.shaderCodePath << "::" << shaderCodeInfo.entrypoint;

	const auto shaderCode = filesystemContext.load(shaderCodeInfo.shaderCodePath);
	auto shaderData =
		milk::graphics::compileShader(*shaderCode, shaderCodeInfo.entrypoint, shaderCodeInfo.shaderType);

	return createShaderFromCompiledShader(
		graphicsRenderer,
		parameterFactory,
		resourceFactory,
		shaderData,
		shaderCodeInfo.shaderType
		);
}

} // anonymous namespace

auto detail::ShaderCreator::doCreate(
	const std::string& id,
	milk::graphics::Renderer& graphicsRenderer,
	const milk::FilesystemContext& filesystemContext
	) -> Instance
{
	CT_LOG_INFO << "Creating shader: \"" << id << "\"";

	if (shaderCodeInfos_.count(id) != 0) {
		CT_LOG_DEBUG << "Found \"" << id << "\" registered as shader code";
		return createShaderFromShaderCode(
			graphicsRenderer,
			filesystemContext,
			parameterFactory_,
			resourceFactory_,
			shaderCodeInfos_[id]
			);
	} else if (compiledShaderInfos_.count(id) != 0) {
		CT_LOG_DEBUG << "Found \"" << id << "\" registered as a compiled shader";
		return createShaderFromCompiledShader(
			graphicsRenderer,
			filesystemContext,
			parameterFactory_,
			resourceFactory_,
			compiledShaderInfos_[id]
			);
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
