#ifndef _COCONUT_MILK_GRAPHICS_DX11_SHADERTYPE_HPP_
#define _COCONUT_MILK_GRAPHICS_DX11_SHADERTYPE_HPP_

#include <coconut-tools/enum.hpp>

struct ID3D11VertexShader;
struct ID3D11PixelShader;

namespace coconut {
namespace milk {
namespace graphics {

CCN_ENUM(ShaderType,
	(VERTEX)
	(PIXEL)
	);

template <class Shader>
constexpr ShaderType fromShader();

template <>
constexpr ShaderType fromShader<ID3D11VertexShader>() {
	return ShaderType::VERTEX;
}

template <>
constexpr ShaderType fromShader<ID3D11PixelShader>() {
	return ShaderType::PIXEL;
}

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_DX11_SHADERTYPE_HPP_ */
