#ifndef _COCONUT_PULP_RENDERER_SHADER_PASSFACTORY_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_PASSFACTORY_HPP_

#include <mutex>
#include <memory>

#include <coconut-tools/factory.hpp>

#include "coconut/milk/graphics/Renderer.hpp"

#include "InputLayoutFactory.hpp"
#include "ShaderFactory.hpp"
#include "Pass.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace shader {

namespace detail {

class PassCreator {
public:

	PassCreator();

	std::unique_ptr<Pass> doCreate(const std::string& passId, milk::graphics::Renderer& graphicsRenderer);

private:

	InputLayoutFactory inputLayoutFactory_;

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
