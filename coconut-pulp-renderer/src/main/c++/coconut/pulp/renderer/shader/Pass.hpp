#ifndef _COCONUT_PULP_RENDERER_SHADER_PASS_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_PASS_HPP_

#include "coconut/milk/graphics/InputLayout.hpp"

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
		milk::graphics::InputLayoutSharedPtr inputLayout,
		VertexShaderSharedPtr vertexShader,
		PixelShaderSharedPtr pixelShader
		) :
		inputLayout_(std::move(inputLayout)),
		vertexShader_(std::move(vertexShader)),
		pixelShader_(std::move(pixelShader))
	{
	}

	const milk::graphics::InputLayoutDescription& inputLayoutDescription() const {
		return inputLayout_->description();
	}

	milk::graphics::InputLayout& inputLayout() {
		return *inputLayout_;
	}

	VertexShader& vertexShader() {
		return *vertexShader_;
	}

	PixelShader& pixelShader() {
		return *pixelShader_;
	}

private:

	milk::graphics::InputLayoutSharedPtr inputLayout_;

	VertexShaderSharedPtr vertexShader_;

	PixelShaderSharedPtr pixelShader_;

};

CCN_MAKE_POINTER_DEFINITIONS(Pass);

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_PASS_HPP_ */
