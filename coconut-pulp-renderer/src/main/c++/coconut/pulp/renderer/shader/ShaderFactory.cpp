#include "ShaderFactory.hpp"

#include <vector>
#include <algorithm>
#include <iterator>

#include <boost/tokenizer.hpp>

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

std::vector<PropertyDescriptor::Object> interpretIdentifier(std::string name) {
	auto result = std::vector<PropertyDescriptor::Object>();

	const auto lastUnderscore = name.find_last_of('_');
	if (lastUnderscore != std::string::npos) {
		name.erase(lastUnderscore);
	}

	auto separator = boost::char_separator<char>("_");
	auto tokenizer = boost::tokenizer<decltype(separator)>(name, separator);

	std::move(tokenizer.begin(), tokenizer.end(), std::back_inserter(result));

	return result;
}

ConstantBuffer::Parameters createParameters(
	const std::string& name,
	const ShaderReflection::Type& type,
	size_t offset,
	std::vector<PropertyDescriptor::Object> descriptor
	)
{
	auto parameters = ConstantBuffer::Parameters();

	{
		auto descriptorTail = interpretIdentifier(name);

		if (descriptorTail.empty()) {
			// TODO: exception
			throw coconut_tools::exceptions::RuntimeError("Invalid identifier " + name);
		}

		descriptor.insert(descriptor.end(), descriptorTail.begin(), descriptorTail.end());

		descriptor.back().arraySize = type.elements;
		descriptor.back().arrayElementOffset = type.elementOffset;
	}

	if (!type.members.empty()) {

		for (const auto& member : type.members) {
			auto memberName = std::string();
			auto memberType = ShaderReflection::Type();
			std::tie(memberName, memberType) = member;

			const auto memberParameters =
				createParameters(memberName, memberType, type.offset + offset, descriptor);
			parameters.reserve(parameters.size() + memberParameters.size());
			std::move(memberParameters.begin(), memberParameters.end(), std::back_inserter(parameters));
		}
	} else {
		CT_LOG_DEBUG << "Creating parameter at offset " << (type.offset + offset)
			<< " reading property " << PropertyDescriptor(descriptor);

		parameters.emplace_back(
			PropertyDescriptor(std::move(descriptor)),
			Property::DataType(type.klass, type.scalarType, type.columns, type.rows),
			type.offset + offset
			);
	}

	return parameters;
}

Resource createResource(
	const ShaderReflection::ResourceInfo& resourceInfo,
	milk::graphics::ShaderType shaderType
	)
{
	auto descriptor = interpretIdentifier(resourceInfo.name);

	CT_LOG_DEBUG
		<< "Creating resource of type " << resourceInfo.type
		<< " for shader type " << shaderType
		<< " at slot " << resourceInfo.slot
		<< " reading property " << PropertyDescriptor(descriptor);

	return Resource(std::move(descriptor), resourceInfo.type, shaderType, resourceInfo.slot);
}

std::unique_ptr<UnknownShader> createShaderFromCompiledShader(
	milk::graphics::Renderer& graphicsRenderer,
	std::vector<std::uint8_t> shaderData,
	milk::graphics::ShaderType shaderType
	)
{
	using milk::graphics::ShaderReflection;

	const auto reflection = ShaderReflection(shaderData.data(), shaderData.size());

	auto constantBuffers = UnknownShader::ConstantBuffers();
	for (const auto& constantBuffer : reflection.constantBuffers()) {
		CT_LOG_DEBUG << "Building parameters for cbuffer " << constantBuffer.name;

		auto bufferParameters = ConstantBuffer::Parameters();

		for (const auto& variable : constantBuffer.variables) {
			auto variableParameters = createParameters(
				variable.name,
				variable.type,
				variable.offset,
				interpretIdentifier(constantBuffer.name)
				);
			std::move(
				variableParameters.begin(),
				variableParameters.end(),
				std::back_inserter(bufferParameters)
				);
		}

		constantBuffers.emplace_back(
			graphicsRenderer,
			shaderType,
			constantBuffer.size,
			constantBuffer.slot,
			std::move(bufferParameters)
			);
	}

	auto resources = UnknownShader::Resources();
	for (const auto& resource : reflection.resources()) {
		resources.emplace_back(createResource(resource, shaderType));
	}

	switch (shaderType) {
	case milk::graphics::ShaderType::VERTEX:
	{
		milk::graphics::VertexShader vs(graphicsRenderer, shaderData.data(), shaderData.size());
		return std::make_unique<VertexShader>(std::move(vs), std::move(constantBuffers), std::move(resources));
	}
	case milk::graphics::ShaderType::GEOMETRY:
	{
		milk::graphics::GeometryShader gs(graphicsRenderer, shaderData.data(), shaderData.size());
		return std::make_unique<GeometryShader>(std::move(gs), std::move(constantBuffers), std::move(resources));
	}
	case milk::graphics::ShaderType::HULL:
	{
		milk::graphics::HullShader hs(graphicsRenderer, shaderData.data(), shaderData.size());
		return std::make_unique<HullShader>(std::move(hs), std::move(constantBuffers), std::move(resources));
	}
	case milk::graphics::ShaderType::DOMAIN:
	{
		milk::graphics::DomainShader ds(graphicsRenderer, shaderData.data(), shaderData.size());
		return std::make_unique<DomainShader>(std::move(ds), std::move(constantBuffers), std::move(resources));
	}
	case milk::graphics::ShaderType::PIXEL:
	{
		milk::graphics::PixelShader ps(graphicsRenderer, shaderData.data(), shaderData.size());
		return std::make_unique<PixelShader>(std::move(ps), std::move(constantBuffers), std::move(resources));
	}
	}

	throw coconut_tools::exceptions::RuntimeError("Unexpected shader type: " + toString(shaderType));
}

std::unique_ptr<UnknownShader> createShaderFromCompiledShader(
	milk::graphics::Renderer& graphicsRenderer,
	const milk::FilesystemContext& filesystemContext,
	const detail::ShaderCreator::CompiledShaderInfo& compiledShaderInfo
	)
{
	return createShaderFromCompiledShader(
		graphicsRenderer,
		*filesystemContext.load(compiledShaderInfo.compiledShaderPath),
		compiledShaderInfo.shaderType
		);
}

std::unique_ptr<UnknownShader> createShaderFromShaderCode(
	milk::graphics::Renderer& graphicsRenderer,
	const milk::FilesystemContext& filesystemContext,
	const detail::ShaderCreator::ShaderCodeInfo& shaderCodeInfo
	)
{
	CT_LOG_INFO << "Compiling shader at " << shaderCodeInfo.shaderCodePath << "::" << shaderCodeInfo.entrypoint;

	const auto shaderCode = filesystemContext.load(shaderCodeInfo.shaderCodePath);
	auto shaderData =
		milk::graphics::compileShader(*shaderCode, shaderCodeInfo.entrypoint, shaderCodeInfo.shaderType);

	return createShaderFromCompiledShader(
		graphicsRenderer,
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
			shaderCodeInfos_[id]
			);
	} else if (compiledShaderInfos_.count(id) != 0) {
		CT_LOG_DEBUG << "Found \"" << id << "\" registered as a compiled shader";
		return createShaderFromCompiledShader(
			graphicsRenderer,
			filesystemContext,
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
