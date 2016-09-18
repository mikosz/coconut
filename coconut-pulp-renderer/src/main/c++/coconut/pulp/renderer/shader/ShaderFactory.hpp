#ifndef _COCONUT_PULP_RENDERER_SHADER_SHADERFACTORY_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_SHADERFACTORY_HPP_

#include <mutex>

#include <coconut-tools/factory.hpp>

#include "coconut/milk/graphics/Renderer.hpp"

#include "Shader.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace shader {

namespace detail {

class ShaderCreator {
public:

	ShaderCreator();

	std::unique_ptr<UnknownShader> doCreate(const std::string& shaderId, milk::graphics::Renderer& graphicsRenderer);

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
