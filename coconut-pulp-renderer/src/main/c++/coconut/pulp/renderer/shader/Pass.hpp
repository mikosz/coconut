#ifndef _COCONUT_PULP_RENDERER_SHADER_PASS_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_PASS_HPP_

#include "Input.hpp"
#include "Shader.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class DrawCommand;
class PassContext;

namespace shader {

class Pass {
public:

	Pass(
		InputSharedPtr input,
		VertexShaderSharedPtr vertexShader,
		PixelShaderSharedPtr pixelShader
		) :
		input_(std::move(input)),
		vertexShader_(std::move(vertexShader)),
		pixelShader_(std::move(pixelShader))
	{
	}

	Input& input() {
		return *input_;
	}

	VertexShader& vertexShader() {
		return *vertexShader_;
	}

	PixelShader& pixelShader() {
		return *pixelShader_;
	}

private:

	InputSharedPtr input_;

	VertexShaderSharedPtr vertexShader_;

	PixelShaderSharedPtr pixelShader_;

};

CCN_MAKE_POINTER_DEFINITIONS(Pass);

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_PASS_HPP_ */
