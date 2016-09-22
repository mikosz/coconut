#ifndef _COCONUT_PULP_RENDERER_SHADER_SHADERFACTORY_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_SHADERFACTORY_HPP_

#include <mutex>
#include <memory>
#include <string>
#include <unordered_map>

#include <boost/filesystem/path.hpp>

#include <coconut-tools/configuration/hierarchical/HierarchicalConfiguration.hpp>
#include <coconut-tools/factory.hpp>

#include "coconut/milk/graphics/Renderer.hpp"
#include "coconut/milk/graphics/ShaderType.hpp"

#include "ParameterFactory.hpp"
#include "Shader.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace shader {

namespace detail {

class ShaderCreator {
public:

	struct ShaderCodeInfo {
		boost::filesystem::path shaderCodePath;
		milk::graphics::ShaderType shaderType;
		std::string entrypoint;
	};

	struct CompiledShaderInfo {
		boost::filesystem::path compiledShaderPath;
		milk::graphics::ShaderType shaderType;
	};

	void registerShaderCode(std::string id, const ShaderCodeInfo& shaderCodeInfo);

	void registerCompiledShader(std::string id, const CompiledShaderInfo& compiledShaderInfo);

	void registerConfig(coconut_tools::configuration::hierarchical::HierarchicalConfigurationSharedPtr config);

	std::unique_ptr<UnknownShader> doCreate(const std::string& id, milk::graphics::Renderer& graphicsRenderer);

private:

	using ShaderCodeInfos = std::unordered_map<std::string, ShaderCodeInfo>;

	using CompiledShaderInfos = std::unordered_map<std::string, CompiledShaderInfo>;

	ShaderCodeInfos shaderCodeInfos_;

	CompiledShaderInfos compiledShaderInfos_;

	ParameterFactory parameterFactory_;

};

} // namespace detail

using ShaderFactory = 
	coconut_tools::Factory<
		std::string,
		UnknownShader,
		coconut_tools::factory::storage::Volatile,
		detail::ShaderCreator,
		std::mutex
		>;

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_SHADERFACTORY_HPP_ */
