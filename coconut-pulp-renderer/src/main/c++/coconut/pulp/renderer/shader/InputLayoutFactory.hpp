#ifndef _COCONUT_PULP_RENDERER_SHADER_INPUTLAYOUTFACTORY_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_INPUTLAYOUTFACTORY_HPP_

#include <string>
#include <mutex>
#include <memory>

#include <unordered_map>

#include <coconut-tools/configuration/hierarchical/HierarchicalConfiguration.hpp>
#include <coconut-tools/factory.hpp>

#include "coconut/milk/graphics/Renderer.hpp"
#include "coconut/milk/graphics/InputLayout.hpp"
#include "coconut/milk/fs.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace shader {

namespace detail {

class InputLayoutCreator {
public:

	struct ShaderCodeInfo {
		milk::AbsolutePath shaderCodePath;
		std::string entrypoint;
	};

	void registerShaderCode(std::string id, const ShaderCodeInfo& shaderCodeInfo);

	void registerCompiledShader(std::string id, milk::AbsolutePath compiledShaderPath);

	void registerConfig(coconut_tools::configuration::hierarchical::HierarchicalConfigurationSharedPtr config);

	std::unique_ptr<milk::graphics::InputLayout> doCreate(
		const std::string& id,
		milk::graphics::Renderer& graphicsRenderer,
		const milk::FilesystemContext& filesystemContext
		);

private:

	using ShaderCodeInfos = std::unordered_map<std::string, ShaderCodeInfo>;

	using CompiledShaderInfos = std::unordered_map<std::string, milk::AbsolutePath>;

	ShaderCodeInfos shaderCodeInfos_;

	CompiledShaderInfos compiledShaderInfos_;

};

} // namespace detail

using InputLayoutFactory = 
	coconut_tools::Factory<
		std::string,
		milk::graphics::InputLayout,
		coconut_tools::factory::storage::Volatile,
		detail::InputLayoutCreator,
		std::mutex
		>;

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_INPUTLAYOUTFACTORY_HPP_ */
