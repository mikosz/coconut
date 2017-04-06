#ifndef _COCONUT_PULP_RENDERER_SHADER_PASS_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_PASS_HPP_

#include "Input.hpp"
#include "Shader.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
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

	const Input& input() const noexcept {
		return *input_;
	}

	Input& input() noexcept {
		return *input_;
	}

	VertexShader& vertexShader() noexcept {
		return *vertexShader_;
	}

	PixelShader& pixelShader() noexcept {
		return *pixelShader_;
	}

	bool isInstanced() const noexcept;

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
