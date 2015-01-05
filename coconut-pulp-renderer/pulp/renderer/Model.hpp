#ifndef _COCONUT_PULP_RENDERER_MODEL_HPP_
#define _COCONUT_PULP_RENDERER_MODEL_HPP_

#include <memory>

#include "milk/graphics/Buffer.hpp"
#include "milk/graphics/Device.hpp"
#include "milk/utils/MakePointerDefinitionsMacro.hpp"
#include "RenderingContext.hpp"
#include "shader/Shader.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class Model {
public:

	Model(milk::graphics::Device& device);

	void render(milk::graphics::Device& devices, RenderingContext renderingContext);

private:

	milk::graphics::Buffer vertexBuffer_;

	milk::graphics::Buffer indexBuffer_;

	shader::ShaderSharedPtr vertexShader_;

	shader::ShaderSharedPtr pixelShader_;

};

MAKE_POINTER_DEFINITIONS(Model);

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_MODEL_HPP_ */
