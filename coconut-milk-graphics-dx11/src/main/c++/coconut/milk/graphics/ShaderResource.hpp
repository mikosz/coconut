#ifndef _COCONUT_MILK_GRAPHICS_SHADERRESOURCE_HPP_
#define _COCONUT_MILK_GRAPHICS_SHADERRESOURCE_HPP_

#include <d3d11.h>

#include "coconut/milk/system/COMWrapper.hpp"

#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

#include "ShaderType.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class Device;

class ShaderResource {
public:

	ShaderResource(system::COMWrapper<ID3D11ShaderResourceView> shaderResourceView) :
		shaderResourceView_(shaderResourceView)
	{
	}

	void bind(Device& device, ShaderType shaderType, size_t slot);

private:

	system::COMWrapper<ID3D11ShaderResourceView> shaderResourceView_;

};

MAKE_POINTER_DEFINITIONS(ShaderResource);

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_SHADERRESOURCE_HPP_ */
