#include "InputLayoutFactory.hpp"

#include <algorithm>
#include <fstream>
#include <memory>

#include "coconut/milk/graphics/ShaderReflection.hpp"
#include "coconut/milk/graphics/compile-shader.hpp"
#include "coconut/milk/graphics/FlexibleInputLayoutDescription.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;
using namespace coconut::pulp::renderer::shader;

namespace /* anonymous */ {

makeFormat

std::unique_ptr<milk::graphics::InputLayout> createInputLayoutFromVertexShader(
	milk::graphics::Renderer& graphicsRenderer, const std::vector<std::uint8_t>& shaderData)
{
	milk::graphics::ShaderReflection reflection(shaderData.data(), shaderData.size());

	const auto& inputParameters = reflection.inputParameters();

	auto description = std::make_unique<milk::graphics::FlexibleInputLayoutDescription>();

	for (const auto& inputParameter : reflection.inputParameters()) {
		using milk::graphics::ShaderReflection;
		using milk::graphics::FlexibleInputLayoutDescription;
		switch (inputParameter.semantic) {
		case ShaderReflection::InputParameterSemantic::POSITION:
			description.push(
				std::make_shared<FlexibleInputLayoutDescription::PositionElement>(
					inputParameter.semanticIndex,
					inputParameter.
					)
				);
		}
	}

	return std::make_unique<milk::graphics::InputLayout>(
		std::move(description), graphicsRenderer, shaderData.data(), shaderData.size()
		);
}

std::unique_ptr<milk::graphics::InputLayout> createInputLayoutFromVertexShader(
	milk::graphics::Renderer& graphicsRenderer, const boost::filesystem::path& path)
{
	std::vector<std::uint8_t> contents;
	std::ifstream ifs(path.string().c_str()); // TODO: this needs to move to some utility place
	std::ostringstream oss;
	oss << ifs.rdbuf();
	auto s = oss.str();
	contents.reserve(s.length());
	std::copy(s.begin(), s.end(), std::back_inserter(contents));

	return createInputLayoutFromVertexShader(graphicsRenderer, contents);
}

std::unique_ptr<milk::graphics::InputLayout> createInputLayoutFromVertexShaderCode(
	milk::graphics::Renderer& graphicsRenderer,
	const boost::filesystem::path& path,
	const std::string& entrypoint
	)
{
	std::vector<std::uint8_t> contents;
	std::ifstream ifs(path.string().c_str()); // TODO: this needs to move to some utility place
	std::ostringstream oss;
	oss << ifs.rdbuf();
	auto s = oss.str();
	contents.reserve(s.length());
	std::copy(s.begin(), s.end(), std::back_inserter(contents));

	auto shaderData = milk::graphics::compileShader(contents, entrypoint);

	return createInputLayoutFromVertexShader(graphicsRenderer, shaderData);
}

} // anonymous namespace

std::unique_ptr<milk::graphics::InputLayout> detail::InputLayoutCreator::doCreate(
	const std::string& id, milk::graphics::Renderer& graphicsRenderer)
{
	if (shaderCodeInfos_.count(id) != 0) {
		const auto& shaderCodeInfo = shaderCodeInfos_[id];
		return createInputLayoutFromVertexShaderCode(
			graphicsRenderer, shaderCodeInfo.shaderCodePath, shaderCodeInfo.entrypoint);
	} else if (compiledShaderPaths_.count(id) != 0) {

	} else {
		throw coconut_tools::factory::error_policy::NoSuchType<std::string>(id);
	}
}

void detail::InputLayoutCreator::registerShaderCode(std::string id, const ShaderCodeInfo& shaderCodeInfo) {
	if (shaderCodeInfos_.count(id) != 0 || compiledShaderPaths_.count(id) != 0) {
		throw coconut_tools::factory::error_policy::CreatorAlreadyRegistered<std::string>(id);
	}

	shaderCodeInfos_.emplace(std::move(id), std::move(shaderCodeInfo));
}

void detail::InputLayoutCreator::registerCompiledShader(std::string id, boost::filesystem::path compiledShaderPath) {
	if (shaderCodeInfos_.count(id) != 0 || compiledShaderPaths_.count(id) != 0) {
		throw coconut_tools::factory::error_policy::CreatorAlreadyRegistered<std::string>(id);
	}

	compiledShaderPaths_.emplace(std::move(id), std::move(compiledShaderPath));
}
