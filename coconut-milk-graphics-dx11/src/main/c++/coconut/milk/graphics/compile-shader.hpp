#ifndef _COCONUT_MILK_GRAPHICS_DX11_COMPILESHADER_HPP_
#define _COCONUT_MILK_GRAPHICS_DX11_COMPILESHADER_HPP_

#include <d3dcommon.h>

#include <functional>
#include <vector>
#include <cstddef>
#include <memory>
#include <iosfwd>
#include <string>
#include "coconut/milk/system/cleanup-windows-macros.hpp"

#include "coconut/milk/system/COMWrapper.hpp"

#include "ShaderType.hpp"

namespace coconut {
namespace milk {
namespace graphics {

using ShaderData = const std::vector<std::uint8_t>;
using ShaderIncludeHandler = std::function<std::shared_ptr<ShaderData>(const std::string&)>;

ShaderData compileShader(
	const std::vector<std::uint8_t>& shaderCode,
	const std::string& entrypoint,
	ShaderType shaderType,
	ShaderIncludeHandler includeHandler
	);

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_DX11_COMPILESHADER_HPP_ */
