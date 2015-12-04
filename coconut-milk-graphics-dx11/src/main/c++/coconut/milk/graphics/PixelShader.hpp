#ifndef _COCONUT_MILK_GRAPHICS_PIXELSHADER_HPP_
#define _COCONUT_MILK_GRAPHICS_PIXELSHADER_HPP_

#include <vector>

#include <d3d11.h>

#include "coconut/milk/system/COMWrapper.hpp"
#include "Shader.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class Device;

class PixelShader : public Shader {
public:

	PixelShader(Device& device, void* data, size_t size);

	void bind(Device& device) const override;

private:

	mutable system::COMWrapper<ID3D11PixelShader> shader_; // because PSSetShader takes PixelShader* not const PixelShader*

};

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_PIXELSHADER_HPP_ */
