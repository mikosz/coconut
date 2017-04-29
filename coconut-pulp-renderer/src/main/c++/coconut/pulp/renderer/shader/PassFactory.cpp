#include "PassFactory.hpp"

#include <coconut-tools/logger.hpp>

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;
using namespace coconut::pulp::renderer::shader;

namespace /* anonymous */ {

CT_LOGGER_CATEGORY("COCONUT.PULP.RENDERER.SHADER.PASS_FACTORY");

} // anonymous namespace

void detail::PassCreator::scanShaderCodeDirectory(
	const milk::fs::FilesystemContext& filesystemContext,
	const milk::fs::Path& directory,
	const std::string& entrypointName
	)
{
	CT_LOG_INFO << "Scanning shader code directory: " << directory;

	const auto names = filesystemContext.list(directory);

	for (const auto& name : names) {
		const auto path = directory / name;

		if (path.extension() == ".hlsl") {
			detail::ShaderCreator::ShaderCodeInfo shaderInfo; // TODO: must expose this type or use different param to constructor (d'uh)
			shaderInfo.shaderCodePath = filesystemContext.makeAbsolute(path);

			const auto shaderName = path.stem();

			if (shaderName.extension() == ".v") {
				detail::InputCreator::ShaderCodeInfo inputInfo;
				inputInfo.shaderCodePath = shaderInfo.shaderCodePath;
				inputInfo.entrypoint = entrypointName;
				inputFactory_.registerShaderCode(shaderName.string(), inputInfo);

				shaderInfo.shaderType = milk::graphics::ShaderType::VERTEX;
				shaderFactory_.registerShaderCode(shaderName.string(), shaderInfo);
			} else if (shaderName.extension() == ".g") {
				shaderInfo.shaderType = milk::graphics::ShaderType::GEOMETRY;
				shaderFactory_.registerShaderCode(shaderName.string(), shaderInfo);
			} else if (shaderName.extension() == ".h") {
				shaderInfo.shaderType = milk::graphics::ShaderType::HULL;
				shaderFactory_.registerShaderCode(shaderName.string(), shaderInfo);
			} else if (shaderName.extension() == ".d") {
				shaderInfo.shaderType = milk::graphics::ShaderType::DOMAIN;
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
				inputFactory_.registerCompiledShader(shaderName.string(), info.compiledShaderPath);

				info.shaderType = milk::graphics::ShaderType::VERTEX;
				shaderFactory_.registerCompiledShader(shaderName.string(), info);
			} else if (shaderName.extension() == ".g") {
				info.shaderType = milk::graphics::ShaderType::GEOMETRY;
				shaderFactory_.registerCompiledShader(shaderName.string(), info);
			} else if (shaderName.extension() == ".h") {
				info.shaderType = milk::graphics::ShaderType::HULL;
				shaderFactory_.registerCompiledShader(shaderName.string(), info);
			} else if (shaderName.extension() == ".d") {
				info.shaderType = milk::graphics::ShaderType::DOMAIN;
				shaderFactory_.registerCompiledShader(shaderName.string(), info);
			} else if (shaderName.extension() == ".p") {
				info.shaderType = milk::graphics::ShaderType::PIXEL;
				shaderFactory_.registerCompiledShader(shaderName.string(), info);
			}
		}
	}
}

auto detail::PassCreator::doCreate(
	const std::string& id,
	milk::graphics::Renderer& graphicsRenderer,
	const milk::fs::FilesystemContext& filesystemContext
	) -> Instance
{
	CT_LOG_INFO << "Creating shader pass: \"" << id << "\"";

	auto geometryShader = GeometryShaderSharedPtr();
	auto hullShader = HullShaderSharedPtr();
	auto domainShader = DomainShaderSharedPtr();

	if (shaderFactory_.hasShader(id + ".g")) {
		geometryShader = std::dynamic_pointer_cast<GeometryShader>(
			shaderFactory_.create(id + ".g", graphicsRenderer, filesystemContext));
	}
	if (shaderFactory_.hasShader(id + ".h")) {
		hullShader = std::dynamic_pointer_cast<HullShader>(
			shaderFactory_.create(id + ".h", graphicsRenderer, filesystemContext));
	}
	if (shaderFactory_.hasShader(id + ".d")) {
		domainShader = std::dynamic_pointer_cast<DomainShader>(
			shaderFactory_.create(id + ".d", graphicsRenderer, filesystemContext));
	}

	return std::make_unique<Pass>(
		false, // TODO: temp - need to discern between instanced and non instanced shaders
		inputFactory_.create(id + ".v", graphicsRenderer, filesystemContext),
		std::dynamic_pointer_cast<VertexShader>(
			shaderFactory_.create(id + ".v", graphicsRenderer, filesystemContext)),
		std::move(geometryShader),
		std::move(hullShader),
		std::move(domainShader),
		std::dynamic_pointer_cast<PixelShader>(
			shaderFactory_.create(id + ".p", graphicsRenderer, filesystemContext))
		);
}
