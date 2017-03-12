#ifndef _COCONUT_PULP_RENDERER_SHADER_PASSFACTORY_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_PASSFACTORY_HPP_

#include <mutex>
#include <memory>

#include <coconut-tools/factory.hpp>

#include "coconut/milk/graphics/Renderer.hpp"

#include "coconut/milk/fs.hpp"

#include "InputFactory.hpp"
#include "ShaderFactory.hpp"
#include "Pass.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace shader {

namespace detail {

class PassCreator {
public:

	std::unique_ptr<Pass> doCreate(
		const std::string& id,
		milk::graphics::Renderer& graphicsRenderer,
		const milk::fs::FilesystemContext& filesystemContext
		);

	void scanShaderCodeDirectory(
		const milk::fs::FilesystemContext& filesystemContext,
		const milk::fs::Path& directory,
		const std::string& entrypointName = "main");

	void scanCompiledShaderDirectory(
		const milk::fs::FilesystemContext& filesystemContext,
		const milk::fs::Path& directory
		);

private:

	InputFactory inputFactory_;

	ShaderFactory shaderFactory_;

};

} // namespace detail

using PassFactory = 
	coconut_tools::Factory<
		std::string,
		Pass,
		coconut_tools::factory::storage::Volatile,
		detail::PassCreator,
		std::mutex
		>;

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_PASSFACTORY_HPP_ */
