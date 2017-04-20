#ifndef _COCONUT_MILK_GRAPHICS_DX11_COMPILESHADER_HPP_
#define _COCONUT_MILK_GRAPHICS_DX11_COMPILESHADER_HPP_

#include <d3dcommon.h>

#include <vector>
#include <cstddef>
#include "coconut/milk/system/cleanup-windows-macros.hpp"

#include "coconut/milk/system/COMWrapper.hpp"

#include "ShaderType.hpp"

namespace coconut {
namespace milk {
namespace graphics {

std::vector<std::uint8_t> compileShader(
	const std::vector<std::uint8_t>& shaderData,
	const std::string& entrypoint,
	ShaderType shaderType
	);

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_DX11_COMPILESHADER_HPP_ */
