#ifndef _COCONUT_PULP_RENDERER_SHADER_SHADERWRAPPER_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_SHADERWRAPPER_HPP_

#include "milk/graphics/Shader.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace shader {

class ShaderWrapper {
public:

	Environment(milk::graphics::ShaderSharedPtr wrappedShader) :
		wrappedShader_(wrappedShader)
	{
	}

	void bind(milk::graphics::Device& device) override {
		wrappedShader_->bind(device);
	}

private:

	milk::graphics::ShaderSharedPtr wrappedShader_;

};

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_SHADERWRAPPER_HPP_ */
