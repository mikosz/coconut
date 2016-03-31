#ifndef _COCONUT_MILK_GRAPHICS_SHADER_HPP_
#define _COCONUT_MILK_GRAPHICS_SHADER_HPP_

#include <memory>

#include <d3d11.h>

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

	Shader(Renderer& renderer, void* data, size_t size);

	InternalShaderType& internalShader() {
		return *shader_;
	}

private:

	system::COMWrapper<InternalShaderType> shader_;

};

} // namespace detail

using VertexShader = detail::Shader<ID3D11VertexShader>;
using PixelShader = detail::Shader<ID3D11PixelShader>;

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_SHADER_HPP_ */
