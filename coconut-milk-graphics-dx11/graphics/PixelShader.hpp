#ifndef _COCONUT_MILK_GRAPHICS_PIXELSHADER_HPP_
#define _COCONUT_MILK_GRAPHICS_PIXELSHADER_HPP_

#include <vector>

#include <d3d11.h>

#include "system/COMWrapper.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class Device;

class PixelShader {
public:

	PixelShader(Device& device, void* data, size_t size);

	void bind(Device& device);

private:

	system::COMWrapper<ID3D11PixelShader> shader_;

};

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_PIXELSHADER_HPP_ */
