#ifndef _COCONUT_PULP_RENDERER_MATERIAL_HPP_
#define _COCONUT_PULP_RENDERER_MATERIAL_HPP_

#include <string>

#include "milk/graphics/Device.hpp"

#include "milk/utils/MakePointerDefinitionsMacro.hpp"

#include "shader/Shader.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

struct RenderingContext;

class Material {
public:

	typedef std::string Id;

	Material(milk::graphics::Device& graphicsDevice);

	void bind(milk::graphics::Device& graphicsDevice, RenderingContext renderingContext);

private:

	shader::ShaderSharedPtr vertexShader_;

	shader::ShaderSharedPtr pixelShader_;

};

MAKE_POINTER_DEFINITIONS(Material);

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_MATERIAL_HPP_ */
