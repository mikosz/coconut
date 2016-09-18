#ifndef _COCONUT_PULP_RENDERER_SHADER_INPUTLAYOUTFACTORY_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_INPUTLAYOUTFACTORY_HPP_

#include <string>
#include <mutex>

#include <unordered_map>

#include <boost/filesystem/path.hpp>

#include <coconut-tools/configuration/hierarchical/HierarchicalConfiguration.hpp>
#include <coconut-tools/factory.hpp>

#include "coconut/milk/graphics/Renderer.hpp"
#include "coconut/milk/graphics/InputLayout.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace shader {

namespace detail {

class InputLayoutCreator {
public:

	struct ShaderCodeInfo {
		boost::filesystem::path shaderCodePath;
		std::string entrypoint;
	};

	void registerShaderCode(std::string id, const ShaderCodeInfo& shaderCodeInfo);

	void registerCompiledShader(std::string id, boost::filesystem::path compiledShaderPath);

	void registerConfig(coconut_tools::configuration::hierarchical::HierarchicalConfigurationSharedPtr config);

	std::unique_ptr<milk::graphics::InputLayout> doCreate(
		const std::string& id, milk::graphics::Renderer& graphicsRenderer);

private:

	using ShaderCodePaths = std::unordered_map<std::string, ShaderCodeInfo>;

	using CompiledShaderPaths = std::unordered_map<std::string, boost::filesystem::path>;

	ShaderCodePaths shaderCodeInfos_;

	CompiledShaderPaths compiledShaderPaths_;

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
