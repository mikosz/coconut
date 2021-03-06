#ifndef _COCONUT_MILK_GRAPHICS_SHADER_HPP_
#define _COCONUT_MILK_GRAPHICS_SHADER_HPP_

#include <memory>

#include <d3d11.h>
#include "coconut/milk/system/cleanup-windows-macros.hpp"

#include "coconut/milk/graphics/ShaderType.hpp"

#include "coconut/milk/system/COMWrapper.hpp"

#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class Renderer;

namespace detail {

template <class InternalShaderType>
class Shader {
public:

	static const auto SHADER_TYPE = shaderTypeFromShader<InternalShaderType>();

	Shader(Renderer& renderer, const void* data, size_t size);

	InternalShaderType& internalShader() {
		return *shader_;
	}

private:

	system::COMWrapper<InternalShaderType> shader_;

};

} // namespace detail

using VertexShader = detail::Shader<ID3D11VertexShader>;
using GeometryShader = detail::Shader<ID3D11GeometryShader>;
using HullShader = detail::Shader<ID3D11HullShader>;
using DomainShader = detail::Shader<ID3D11DomainShader>;
using PixelShader = detail::Shader<ID3D11PixelShader>;

CT_MAKE_POINTER_DEFINITIONS(VertexShader);
CT_MAKE_POINTER_DEFINITIONS(GeometryShader);
CT_MAKE_POINTER_DEFINITIONS(HullShader);
CT_MAKE_POINTER_DEFINITIONS(DomainShader);
CT_MAKE_POINTER_DEFINITIONS(PixelShader);

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_SHADER_HPP_ */
