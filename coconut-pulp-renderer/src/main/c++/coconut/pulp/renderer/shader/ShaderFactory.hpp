#ifndef _COCONUT_PULP_RENDERER_SHADER_SHADERFACTORY_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_SHADERFACTORY_HPP_

#include <string>

#include "coconut/milk/graphics/Device.hpp"

#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace shader {

class Shader;
MAKE_POINTER_DEFINITIONS(Shader);

class ShaderFactory {
public:

	typedef std::string ShaderId;

	ShaderFactory();

	ShaderSharedPtr createShader(milk::graphics::Device& graphicsDevice, ShaderId shaderId);

};

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_SHADERFACTORY_HPP_ */
