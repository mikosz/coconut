#ifndef _COCONUT_PULP_RENDERER_SHADER_INPUTFACTORY_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_INPUTFACTORY_HPP_

#include <string>
#include <mutex>
#include <memory>

#include <unordered_map>

#include <coconut-tools/factory.hpp>

#include "coconut/milk/graphics/Renderer.hpp"
#include "coconut/milk/fs.hpp"

#include "Input.hpp"
#include "InputElementFactory.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace shader {

namespace detail {

class InputCreator {
public:

	using Instance = std::unique_ptr<Input>;

	struct ShaderCodeInfo {
		milk::AbsolutePath shaderCodePath;
		std::string entrypoint;
	};

	// TODO: duplicated code with ShaderCreator, could we re-use somehow?
	void registerShaderCode(std::string id, const ShaderCodeInfo& shaderCodeInfo);

	void registerCompiledShader(std::string id, milk::AbsolutePath compiledShaderPath);

protected:

	Instance doCreate(
		const std::string& id,
		milk::graphics::Renderer& graphicsRenderer,
		const milk::FilesystemContext& filesystemContext
		);

private:

	using ShaderCodeInfos = std::unordered_map<std::string, ShaderCodeInfo>;

	using CompiledShaderInfos = std::unordered_map<std::string, milk::AbsolutePath>;

	ShaderCodeInfos shaderCodeInfos_;

	CompiledShaderInfos compiledShaderInfos_;

	InputElementFactory inputElementFactory_;

};

} // namespace detail

using InputFactory = 
	coconut_tools::Factory<
		std::string,
		detail::InputCreator,
		coconut_tools::factory::storage::Volatile,
		std::mutex
		>;

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_INPUTFACTORY_HPP_ */
