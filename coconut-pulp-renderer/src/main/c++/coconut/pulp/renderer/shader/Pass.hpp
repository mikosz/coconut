#ifndef _COCONUT_PULP_RENDERER_SHADER_PASS_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_PASS_HPP_

#include "coconut/milk/graphics/InputLayout.hpp"

#include "Shader.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

struct DrawCommand;
struct RenderingContext;

namespace shader {

class Pass {
public:

	Pass(
		milk::graphics::InputLayoutUniquePtr inputLayout,
		ShaderUniquePtr vertexShader,
		ShaderUniquePtr pixelShader
		) :
		inputLayout_(std::move(inputLayout)),
		vertexShader_(std::move(vertexShader)),
		pixelShader_(std::move(pixelShader))
	{
	}

	std::unique_ptr<DrawCommand> createDrawCommand(const RenderingContext& renderingContext) const;

	const milk::graphics::InputLayoutDescription& inputLayoutDescription() const {
		return inputLayout_->description();
	}

private:

	milk::graphics::InputLayoutUniquePtr inputLayout_;

	ShaderUniquePtr vertexShader_;

	ShaderUniquePtr pixelShader_;

};

CCN_MAKE_POINTER_DEFINITIONS(Pass);

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_PASS_HPP_ */
