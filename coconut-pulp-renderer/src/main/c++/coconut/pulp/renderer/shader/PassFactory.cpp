#include "PassFactory.hpp"

#include <coconut-tools/logger.hpp>

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;
using namespace coconut::pulp::renderer::shader;

namespace /* anonymous */ {

CT_LOGGER_CATEGORY("COCONUT.PULP.RENDERER.SHADER.PASS_FACTORY");

} // anonymous namespace

std::unique_ptr<Pass> detail::PassCreator::doCreate(
	const std::string& id,
	milk::graphics::Renderer& graphicsRenderer,
	const milk::fs::FilesystemContext& filesystemContext
	)
{
	CT_LOG_INFO << "Creating shader pass: \"" << id << "\"";

	return std::make_unique<Pass>(
		inputLayoutFactory_.create(id + ".v", graphicsRenderer, filesystemContext),
		std::dynamic_pointer_cast<VertexShader>(
			shaderFactory_.create(id + ".v", graphicsRenderer, filesystemContext)),
		std::dynamic_pointer_cast<PixelShader>(
			shaderFactory_.create(id + ".p", graphicsRenderer, filesystemContext))
		);
}

void detail::PassCreator::scanShaderCodeDirectory(
	const milk::fs::FilesystemContext& filesystemContext,
	const milk::fs::Path& directory,
	const std::string& entrypointName
	)
{
	CT_LOG_INFO << "Scanning shader code directory: " << directory;

	const auto names = filesystemContext.list(directory);

	for (const auto& name : names) {
		auto path = directory / name;

		if (path.extension() == ".hlsl") {
			detail::ShaderCreator::ShaderCodeInfo shaderInfo; // TODO: must expose this type or use different param to constructor (d'uh)
			shaderInfo.shaderCodePath = filesystemContext.makeAbsolute(path);

			const auto shaderName = path.stem();

			if (shaderName.extension() == ".v") {
				detail::InputLayoutCreator::ShaderCodeInfo inputLayoutInfo;
				inputLayoutInfo.shaderCodePath = shaderInfo.shaderCodePath;
				inputLayoutInfo.entrypoint = entrypointName;
				inputLayoutFactory_.registerShaderCode(shaderName.string(), inputLayoutInfo);

				shaderInfo.shaderType = milk::graphics::ShaderType::VERTEX;
				shaderFactory_.registerShaderCode(shaderName.string(), shaderInfo);
			} else if (shaderName.extension() == ".p") {
				shaderInfo.shaderType = milk::graphics::ShaderType::PIXEL;
				shaderFactory_.registerShaderCode(shaderName.string(), shaderInfo);
			}
		}
	}
}

void detail::PassCreator::scanCompiledShaderDirectory(
	const milk::fs::FilesystemContext& filesystemContext,
	const milk::fs::Path& directory
	)
{
	CT_LOG_INFO << "Scanning compiled shader directory: " << directory;

	const auto names = filesystemContext.list(directory);

	for (const auto& name : names) {
		auto path = directory / name;
		
		if (path.extension() == ".cso") {
			detail::ShaderCreator::CompiledShaderInfo info; // TODO: must expose this type or use different param to constructor (d'uh)
			info.compiledShaderPath = filesystemContext.makeAbsolute(path);

			const auto shaderName = path.stem();

			if (shaderName.extension() == ".v") {
				inputLayoutFactory_.registerCompiledShader(shaderName.string(), info.compiledShaderPath);

				info.shaderType = milk::graphics::ShaderType::VERTEX;
				shaderFactory_.registerCompiledShader(shaderName.string(), info);
			} else if (shaderName.extension() == ".p") {
				info.shaderType = milk::graphics::ShaderType::PIXEL;
				shaderFactory_.registerCompiledShader(shaderName.string(), info);
			}
		}
	}
}
