#include "InputLayoutFactory.hpp"

#include <algorithm>
#include <fstream>
#include <memory>

#include <coconut-tools/logger.hpp>
#include <coconut-tools/exceptions/RuntimeError.hpp>

#include "coconut/milk/graphics/ShaderReflection.hpp"
#include "coconut/milk/graphics/compile-shader.hpp"
#include "coconut/milk/graphics/FlexibleInputLayoutDescription.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;
using namespace coconut::pulp::renderer::shader;

namespace /* anonymous */ {

CT_LOGGER_CATEGORY("COCONUT.PULP.RENDERER.SHADER.INPUT_LAYOUT_FACTORY");

milk::graphics::FlexibleInputLayoutDescription::Format makeFormat(
	milk::graphics::ShaderReflection::InputParameterInfo::DataType dataType,
	size_t elements
	)
{
	using milk::graphics::ShaderReflection;
	using milk::graphics::FlexibleInputLayoutDescription;

	switch (dataType) {
	case ShaderReflection::InputParameterInfo::DataType::FLOAT:
		switch (elements) {
		case 2:
			return FlexibleInputLayoutDescription::Format::R32G32_FLOAT;
		case 3:
			return FlexibleInputLayoutDescription::Format::R32G32B32_FLOAT;
		case 4:
			return FlexibleInputLayoutDescription::Format::R32G32B32A32_FLOAT;
		}
	}

	throw coconut_tools::exceptions::RuntimeError(
		"Unsupported input parameter format: " + toString(dataType) + "x" + std::to_string(elements)
		);
}

std::unique_ptr<milk::graphics::InputLayout> createInputLayoutFromVertexShader(
	milk::graphics::Renderer& graphicsRenderer, std::vector<std::uint8_t>& shaderData)
{
	milk::graphics::ShaderReflection reflection(shaderData.data(), shaderData.size());

	const auto& inputParameters = reflection.inputParameters();

	auto description = std::make_unique<milk::graphics::FlexibleInputLayoutDescription>();

	for (const auto& inputParameter : reflection.inputParameters()) {
		using milk::graphics::ShaderReflection;
		using milk::graphics::FlexibleInputLayoutDescription;

		auto format = makeFormat(inputParameter.dataType, inputParameter.elements);

		switch (inputParameter.semantic) {
		case ShaderReflection::InputParameterInfo::Semantic::POSITION:
			description->push(
				std::make_shared<FlexibleInputLayoutDescription::PositionElement>(inputParameter.semanticIndex, format));
			break;
		case ShaderReflection::InputParameterInfo::Semantic::NORMAL:
			description->push(
				std::make_shared<FlexibleInputLayoutDescription::NormalElement>(inputParameter.semanticIndex, format));
			break;
		case ShaderReflection::InputParameterInfo::Semantic::TEXCOORD:
			description->push(
				std::make_shared<FlexibleInputLayoutDescription::TextureCoordinatesElement>(inputParameter.semanticIndex, format));
			break;
		}
	}

	return std::make_unique<milk::graphics::InputLayout>(
		std::move(description), graphicsRenderer, shaderData.data(), shaderData.size()
		);
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

std::unique_ptr<milk::graphics::InputLayout> createInputLayoutFromVertexShader(
	milk::graphics::Renderer& graphicsRenderer, const boost::filesystem::path& path)
{
	return createInputLayoutFromVertexShader(graphicsRenderer, readContents(path));
}

std::unique_ptr<milk::graphics::InputLayout> createInputLayoutFromVertexShaderCode(
	milk::graphics::Renderer& graphicsRenderer,
	const boost::filesystem::path& path,
	const std::string& entrypoint
	)
{
	auto shaderData = milk::graphics::compileShader(readContents(path), entrypoint);

	return createInputLayoutFromVertexShader(graphicsRenderer, shaderData);
}

} // anonymous namespace

std::unique_ptr<milk::graphics::InputLayout> detail::InputLayoutCreator::doCreate(
	const std::string& id, milk::graphics::Renderer& graphicsRenderer)
{
	CT_LOG_INFO << "Creating input layout: \"" << id << "\"";

	if (shaderCodeInfos_.count(id) != 0) {
		CT_LOG_DEBUG << "Found \"" << id << "\" registered as shader code";
		const auto& shaderCodeInfo = shaderCodeInfos_[id];
		return createInputLayoutFromVertexShaderCode(
			graphicsRenderer, shaderCodeInfo.shaderCodePath, shaderCodeInfo.entrypoint);
	} else if (compiledShaderPaths_.count(id) != 0) {
		CT_LOG_DEBUG << "Found \"" << id << "\" registered as a compiled shader";
		return createInputLayoutFromVertexShader(graphicsRenderer, compiledShaderPaths_[id]);
	} else {
		throw coconut_tools::factory::error_policy::NoSuchType<std::string>(id);
	}
}

void detail::InputLayoutCreator::registerShaderCode(std::string id, const ShaderCodeInfo& shaderCodeInfo) {
	CT_LOG_INFO << "Registering shader code \"" << id << "\" at " << shaderCodeInfo.shaderCodePath.string()
		<< "::" << shaderCodeInfo.entrypoint << " for input analysis";

	if (shaderCodeInfos_.count(id) != 0 || compiledShaderPaths_.count(id) != 0) {
		throw coconut_tools::factory::error_policy::CreatorAlreadyRegistered<std::string>(id);
	}

	shaderCodeInfos_.emplace(std::move(id), std::move(shaderCodeInfo));
}

void detail::InputLayoutCreator::registerCompiledShader(std::string id, boost::filesystem::path compiledShaderPath) {
	CT_LOG_INFO << "Registering compiled shader \"" << id << "\" at " << compiledShaderPath.string()
		<< " for input analysis";

	if (shaderCodeInfos_.count(id) != 0 || compiledShaderPaths_.count(id) != 0) {
		throw coconut_tools::factory::error_policy::CreatorAlreadyRegistered<std::string>(id);
	}

	compiledShaderPaths_.emplace(std::move(id), std::move(compiledShaderPath));
}
