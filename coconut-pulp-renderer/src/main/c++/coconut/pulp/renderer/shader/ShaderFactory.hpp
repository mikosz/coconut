#ifndef _COCONUT_PULP_RENDERER_SHADER_SHADERFACTORY_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_SHADERFACTORY_HPP_

#include <string>

#include "coconut/milk/graphics/Device.hpp"

#include "Pass.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace shader {

class ShaderFactory {
public:

	using ShaderId = std::string;

	using PassId = std::string;

	ShaderFactory();

	PassUniquePtr createShaderPass(milk::graphics::Device& graphicsDevice, PassId passId);

	ShaderUniquePtr createShader(milk::graphics::Device& graphicsDevice, ShaderId shaderId);

private:

	

};

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_SHADERFACTORY_HPP_ */
