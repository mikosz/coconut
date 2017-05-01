#include "ShaderFactory.hpp"

#include <vector>
#include <algorithm>

#include <coconut-tools/logger.hpp>
#include <coconut-tools/exceptions/RuntimeError.hpp>

#include "coconut/milk/graphics/compile-shader.hpp"
#include "coconut/milk/graphics/ShaderReflection.hpp"

#include "Property.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;
using namespace coconut::pulp::renderer::shader;

namespace /* anonymous */ {

CT_LOGGER_CATEGORY("COCONUT.PULP.RENDERER.SHADER.SHADER_FACTORY");

using milk::graphics::ShaderReflection;

std::vector<Parameter> createParameters(
	const std::string& name,
	const ShaderReflection::Type& type,
	size_t offset,
	std::vector<PropertyDescriptor::Object> prefix
	)
{
	CT_LOG_DEBUG << "Creating parameter for variable " << name;

	const auto offset = offset + type.offset;

	auto parameters = std::vector<Parameter>();

	if (!type.members.empty()) {
		prefix.emplace_back(name, 0u); // TODO: temp, arrays!
		for (const auto& member : type.members) {
			auto memberName = std::string();
			auto memberType = ShaderReflection::Type();
			std::tie(memberName, memberType) = member;

			parameters = createParameters(memberName, memberType, type.offset, prefix);
		}
	} else {
		parameters.emplace_back(
			PropertyDescriptor(std::move(prefix), name),
			Property::DataType(type.klass, type.scalarType),
			offset
			);
	}

	return parameters;
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
	ResourceFactory& resourceFactory,
	std::vector<std::uint8_t> shaderData,
	milk::graphics::ShaderType shaderType
	)
{
	using milk::graphics::ShaderReflection;

	const auto reflection = ShaderReflection(shaderData.data(), shaderData.size());

	auto parameters = std::vector<Parameter>();
	for (const auto& constantBuffer : reflection.constantBuffers()) {
		for (const auto& variable : constantBuffer.variables) {
			auto parameters = createParameters(variable.name, variable.type, variable.offset, {});
			std::move(parameters.begin(), parameters.end(), std::back_inserter(parameters));
		}
	}

	auto resources = UnknownShader::Resources();
	for (const auto& resource : reflection.resources()) {
		resources.emplace_back(createResource(resourceFactory, resource, shaderType));
	}

	switch (shaderType) {
	case milk::graphics::ShaderType::VERTEX:
	{
		milk::graphics::VertexShader vs(graphicsRenderer, shaderData.data(), shaderData.size());
		return std::make_unique<VertexShader>(std::move(vs), std::move(parameters), std::move(resources));
	}
	case milk::graphics::ShaderType::GEOMETRY:
	{
		milk::graphics::GeometryShader gs(graphicsRenderer, shaderData.data(), shaderData.size());
		return std::make_unique<GeometryShader>(std::move(gs), std::move(parameters), std::move(resources));
	}
	case milk::graphics::ShaderType::HULL:
	{
		milk::graphics::HullShader hs(graphicsRenderer, shaderData.data(), shaderData.size());
		return std::make_unique<HullShader>(std::move(hs), std::move(parameters), std::move(resources));
	}
	case milk::graphics::ShaderType::DOMAIN:
	{
		milk::graphics::DomainShader ds(graphicsRenderer, shaderData.data(), shaderData.size());
		return std::make_unique<DomainShader>(std::move(ds), std::move(parameters), std::move(resources));
	}
	case milk::graphics::ShaderType::PIXEL:
	{
		milk::graphics::PixelShader ps(graphicsRenderer, shaderData.data(), shaderData.size());
		return std::make_unique<PixelShader>(std::move(ps), std::move(parameters), std::move(resources));
	}
	}

	throw coconut_tools::exceptions::RuntimeError("Unexpected shader type: " + toString(shaderType));
}

std::unique_ptr<UnknownShader> createShaderFromCompiledShader(
	milk::graphics::Renderer& graphicsRenderer,
	const milk::FilesystemContext& filesystemContext,
	ResourceFactory& resourceFactory,
	const detail::ShaderCreator::CompiledShaderInfo& compiledShaderInfo
	)
{
	return createShaderFromCompiledShader(
		graphicsRenderer,
		resourceFactory,
		*filesystemContext.load(compiledShaderInfo.compiledShaderPath),
		compiledShaderInfo.shaderType
		);
}

std::unique_ptr<UnknownShader> createShaderFromShaderCode(
	milk::graphics::Renderer& graphicsRenderer,
	const milk::FilesystemContext& filesystemContext,
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
			resourceFactory_,
			shaderCodeInfos_[id]
			);
	} else if (compiledShaderInfos_.count(id) != 0) {
		CT_LOG_DEBUG << "Found \"" << id << "\" registered as a compiled shader";
		return createShaderFromCompiledShader(
			graphicsRenderer,
			filesystemContext,
			resourceFactory_,
			compiledShaderInfos_[id]
			);
	} else {
		throw coconut_tools::factory::error_policy::NoSuchType<std::string>(id);
	}
}

bool detail::ShaderCreator::hasShader(const std::string& id) const noexcept {
	return shaderCodeInfos_.count(id) > 0 || compiledShaderInfos_.count(id) > 0;
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
