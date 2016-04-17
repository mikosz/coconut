#ifndef _COCONUT_PULP_RENDERER_SHADER_SHADERFACTORY_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_SHADERFACTORY_HPP_

#include <string>

#include "coconut/milk/graphics/Renderer.hpp"

#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

#include "Pass.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace shader {

class ShaderFactory {
public:

	typedef std::string ShaderId;

	ShaderFactory();

	PassUniquePtr createShader(milk::graphics::Renderer& graphicsRenderer, ShaderId shaderId);

};

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_SHADERFACTORY_HPP_ */
