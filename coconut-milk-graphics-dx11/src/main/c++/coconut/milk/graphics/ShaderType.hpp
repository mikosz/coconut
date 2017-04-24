#ifndef _COCONUT_MILK_GRAPHICS_DX11_SHADERTYPE_HPP_
#define _COCONUT_MILK_GRAPHICS_DX11_SHADERTYPE_HPP_

#include <coconut-tools/enum.hpp>

struct ID3D11VertexShader;
struct ID3D11GeometryShader;
struct ID3D11HullShader;
struct ID3D11DomainShader;
struct ID3D11PixelShader;

namespace coconut {
namespace milk {
namespace graphics {

CCN_ENUM(ShaderType,
	(VERTEX)
	(GEOMETRY)
	(HULL)
	(DOMAIN)
	(PIXEL)
	);

template <class Shader>
constexpr ShaderType shaderTypeFromShader();

template <>
constexpr ShaderType shaderTypeFromShader<ID3D11VertexShader>() {
	return ShaderType::VERTEX;
}

template <>
constexpr ShaderType shaderTypeFromShader<ID3D11GeometryShader>() {
	return ShaderType::GEOMETRY;
}

template <>
constexpr ShaderType shaderTypeFromShader<ID3D11HullShader>() {
	return ShaderType::HULL;
}

template <>
constexpr ShaderType shaderTypeFromShader<ID3D11DomainShader>() {
	return ShaderType::DOMAIN;
}

template <>
constexpr ShaderType shaderTypeFromShader<ID3D11PixelShader>() {
	return ShaderType::PIXEL;
}

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_DX11_SHADERTYPE_HPP_ */
