#include "PassFactory.hpp"

#include <algorithm>

#include <boost/filesystem.hpp>

#include <coconut-tools/logger.hpp>

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;
using namespace coconut::pulp::renderer::shader;

namespace /* anonymous */ {

CT_LOGGER_CATEGORY("COCONUT.PULP.RENDERER.SHADER.PASS_FACTORY");

} // anonymous namespace

std::unique_ptr<Pass> detail::PassCreator::doCreate(
	const std::string& id, milk::graphics::Renderer& graphicsRenderer)
{
	CT_LOG_INFO << "Creating shader pass: \"" << id << "\"";

	return std::make_unique<Pass>(
		inputLayoutFactory_.create(id + ".v", graphicsRenderer),
		std::dynamic_pointer_cast<VertexShader>(shaderFactory_.create(id + ".v", graphicsRenderer)),
		std::dynamic_pointer_cast<PixelShader>(shaderFactory_.create(id + ".p", graphicsRenderer))
		);
}

void detail::PassCreator::scanShaderCodeDirectory(const boost::filesystem::path& directory, const std::string& entrypointName) {
	CT_LOG_INFO << "Scanning shader code directory: " << directory;

	boost::filesystem::directory_iterator fileIt(directory), filesEnd;

	std::for_each(fileIt, filesEnd, [this, &entrypointName](const boost::filesystem::path& path) {
			if (path.extension() == ".hlsl") {
				detail::ShaderCreator::ShaderCodeInfo shaderInfo; // TODO: must expose this type or use different param to constructor (d'uh)
				shaderInfo.shaderCodePath = path;

				const auto shaderName = path.stem();

				if (shaderName.extension() == ".v") {
					detail::InputLayoutCreator::ShaderCodeInfo inputLayoutInfo;
					inputLayoutInfo.shaderCodePath = path;
					inputLayoutInfo.entrypoint = entrypointName;
					inputLayoutFactory_.registerShaderCode(shaderName.string(), inputLayoutInfo);

					shaderInfo.shaderType = milk::graphics::ShaderType::VERTEX;
					shaderFactory_.registerShaderCode(shaderName.string(), shaderInfo);
				} else if (shaderName.extension() == ".p") {
					shaderInfo.shaderType = milk::graphics::ShaderType::PIXEL;
					shaderFactory_.registerShaderCode(shaderName.string(), shaderInfo);
				}
			}
		});
}

void detail::PassCreator::scanCompiledShaderDirectory(const boost::filesystem::path& directory) {
	CT_LOG_INFO << "Scanning compiled shader directory: " << directory;

	boost::filesystem::directory_iterator fileIt(directory), filesEnd;

	std::for_each(fileIt, filesEnd, [this](const boost::filesystem::path& path) {
			if (path.extension() == ".cso") {
				detail::ShaderCreator::CompiledShaderInfo info; // TODO: must expose this type or use different param to constructor (d'uh)
				info.compiledShaderPath = path;

				const auto shaderName = path.stem();

				if (shaderName.extension() == ".v") {
					inputLayoutFactory_.registerCompiledShader(shaderName.string(), path);

					info.shaderType = milk::graphics::ShaderType::VERTEX;
					shaderFactory_.registerCompiledShader(shaderName.string(), info);
				} else if (shaderName.extension() == ".p") {
					info.shaderType = milk::graphics::ShaderType::PIXEL;
					shaderFactory_.registerCompiledShader(shaderName.string(), info);
				}
			}
		});
}
