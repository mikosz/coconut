#include "InputFactory.hpp"

#include <algorithm>
#include <memory>

#include <boost/algorithm/string/predicate.hpp>

#include <coconut-tools/logger.hpp>
#include <coconut-tools/exceptions/RuntimeError.hpp>

#include "coconut/milk/graphics/ShaderReflection.hpp"
#include "coconut/milk/graphics/compile-shader.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;
using namespace coconut::pulp::renderer::shader;

using namespace std::string_literals;

namespace /* anonymous */ {

CT_LOGGER_CATEGORY("COCONUT.PULP.RENDERER.SHADER.INPUT_LAYOUT_FACTORY");

milk::graphics::PixelFormat makeFormat( // TODO: could be done by a smarter pixel format type
	milk::graphics::ShaderReflection::InputParameterInfo::DataType dataType,
	size_t elements
	)
{
	using milk::graphics::ShaderReflection;
	using milk::graphics::PixelFormat;

	switch (dataType) {
	case ShaderReflection::InputParameterInfo::DataType::FLOAT:
		switch (elements) {
		case 2:
			return PixelFormat::R32G32_FLOAT;
		case 3:
			return PixelFormat::R32G32B32_FLOAT;
		case 4:
			return PixelFormat::R32G32B32A32_FLOAT;
		}
	}

	throw coconut_tools::exceptions::RuntimeError(
		"Unsupported input parameter format: " + toString(dataType) + "x" + std::to_string(elements)
		);
}

std::unique_ptr<Input> createInputFromVertexShader(
	milk::graphics::Renderer& graphicsRenderer,
	InputElementFactory& inputElementFactory,
	std::vector<std::uint8_t>& shaderData
	)
{
	using milk::graphics::ShaderReflection;
	auto reflection = ShaderReflection(shaderData.data(), shaderData.size());
	const auto& inputParameters = reflection.inputParameters();

	auto elements = Input::Elements();

	for (const auto& inputParameter : reflection.inputParameters()) {
		auto format = makeFormat(inputParameter.dataType, inputParameter.elements);
		auto inputElementDetails = InputElementFactoryInstanceDetails(
			inputParameter.semantic,
			inputParameter.semanticIndex,
			format
			);

		elements.emplace_back(
			*inputElementFactory.create(inputElementDetails, inputElementDetails) // TODO: uhh... could we avoid specyfing this argument twice?
			);
	}

	return std::make_unique<Input>(graphicsRenderer, std::move(elements));
}

std::unique_ptr<Input> createInputFromVertexShaderCode(
	milk::graphics::Renderer& graphicsRenderer,
	InputElementFactory& inputElementFactory,
	const milk::RawData& shaderCode,
	const std::string& entrypoint
	)
{
	auto shaderData =
		milk::graphics::compileShader(shaderCode, entrypoint, milk::graphics::ShaderType::VERTEX);
	return createInputFromVertexShader(graphicsRenderer, inputElementFactory, shaderData);
}

} // anonymous namespace

std::unique_ptr<Input> detail::InputCreator::doCreate(
	const std::string& id,
	milk::graphics::Renderer& graphicsRenderer,
	const milk::FilesystemContext& filesystemContext
	)
{
	CT_LOG_INFO << "Creating input for shader: \"" << id << "\"";

	if (shaderCodeInfos_.count(id) != 0) {
		CT_LOG_DEBUG << "Found \"" << id << "\" registered as shader code";
		const auto& shaderCodeInfo = shaderCodeInfos_[id];
		return createInputFromVertexShaderCode(
			graphicsRenderer,
			inputElementFactory_,
			*filesystemContext.load(shaderCodeInfo.shaderCodePath),
			shaderCodeInfo.entrypoint
			);
	} else if (compiledShaderInfos_.count(id) != 0) {
		CT_LOG_DEBUG << "Found \"" << id << "\" registered as a compiled shader";
		auto shaderData = *filesystemContext.load(compiledShaderInfos_[id]);
		return createInputFromVertexShader(graphicsRenderer, inputElementFactory_, shaderData);
	} else {
		throw coconut_tools::factory::error_policy::NoSuchType<std::string>(id);
	}
}

void detail::InputCreator::registerShaderCode(std::string id, const ShaderCodeInfo& shaderCodeInfo) {
	CT_LOG_INFO << "Registering shader code \"" << id << "\" at " << shaderCodeInfo.shaderCodePath.string()
		<< "::" << shaderCodeInfo.entrypoint << " for input analysis";

	if (shaderCodeInfos_.count(id) != 0 || compiledShaderInfos_.count(id) != 0) {
		throw coconut_tools::factory::error_policy::CreatorAlreadyRegistered<std::string>(id);
	}

	shaderCodeInfos_.emplace(std::move(id), std::move(shaderCodeInfo));
}

void detail::InputCreator::registerCompiledShader(std::string id, milk::AbsolutePath compiledShaderPath) {
	CT_LOG_INFO << "Registering compiled shader \"" << id << "\" at " << compiledShaderPath.string()
		<< " for input analysis";

	if (shaderCodeInfos_.count(id) != 0 || compiledShaderInfos_.count(id) != 0) {
		throw coconut_tools::factory::error_policy::CreatorAlreadyRegistered<std::string>(id);
	}

	compiledShaderInfos_.emplace(std::move(id), std::move(compiledShaderPath));
}
