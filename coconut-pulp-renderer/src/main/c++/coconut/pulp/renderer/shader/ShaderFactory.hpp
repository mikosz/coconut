#ifndef _COCONUT_PULP_RENDERER_SHADER_SHADERFACTORY_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_SHADERFACTORY_HPP_

#include <mutex>
#include <memory>
#include <string>
#include <unordered_map>

#include <coconut-tools/factory.hpp>

#include "coconut/milk/graphics/Renderer.hpp"
#include "coconut/milk/graphics/ShaderType.hpp"
#include "coconut/milk/graphics/ShaderCompiler.hpp"
#include "coconut/milk/fs.hpp"
#include "Input.hpp"
#include "Shader.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace shader {

namespace detail {

class ShaderCreator {
public:

	using Instance = std::unique_ptr<UnknownShader>;

	struct ShaderCodeInfo {
		milk::AbsolutePath shaderCodePath;
		milk::graphics::ShaderType shaderType;
		std::string entrypoint;
	};

	struct CompiledShaderInfo {
		milk::AbsolutePath compiledShaderPath;
		milk::graphics::ShaderType shaderType;
	};

    ShaderCreator();

	Input createInput(
		const std::string& id,
		milk::graphics::Renderer& graphicsRenderer,
		const milk::FilesystemContext& filesystemContext
		) const;

	bool hasShader(const std::string& id) const noexcept;

	void registerShaderCode(std::string id, const ShaderCodeInfo& shaderCodeInfo);

	void registerCompiledShader(std::string id, const CompiledShaderInfo& compiledShaderInfo);

protected:

	Instance doCreate(
		const std::string& id,
		milk::graphics::Renderer& graphicsRenderer,
		const milk::FilesystemContext& filesystemContext
		);

private:

	using ShaderCodeInfos = std::unordered_map<std::string, ShaderCodeInfo>;

	using CompiledShaderInfos = std::unordered_map<std::string, CompiledShaderInfo>;

    milk::graphics::ShaderCompiler shaderCompiler_;

	ShaderCodeInfos shaderCodeInfos_;

	CompiledShaderInfos compiledShaderInfos_;

	std::tuple<std::vector<std::uint8_t>, milk::graphics::ShaderType> shaderCode_(
		const std::string& id,
		const milk::FilesystemContext& filesystemContext
		) const;

};

} // namespace detail

using ShaderFactory = 
	coconut_tools::Factory<
		std::string,
		detail::ShaderCreator,
		coconut_tools::factory::storage::Volatile,
		std::mutex
		>;

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_SHADERFACTORY_HPP_ */
