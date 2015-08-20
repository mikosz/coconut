#ifndef _COCONUT_MILK_GRAPHICS_DX11_SHADER_HPP_
#define _COCONUT_MILK_GRAPHICS_DX11_SHADER_HPP_

#include <memory>

#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class Device;

class Shader {
public:

	virtual void bind(Device& device) = 0;

protected:

	virtual ~Shader() {
	}

};

MAKE_POINTER_DEFINITIONS(Shader);

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_DX11_SHADER_HPP_ */
