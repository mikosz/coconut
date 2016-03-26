#ifndef _COCONUT_MILK_GRAPHICS_VERTEXSHADER_HPP_
#define _COCONUT_MILK_GRAPHICS_VERTEXSHADER_HPP_

#include <memory>
#include <vector>

#include <d3d11.h>

#include "coconut/milk/system/COMWrapper.hpp"
#include "InputLayout.hpp"
#include "Buffer.hpp"
#include "Shader.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class Device;

class VertexShader : public Shader {
public:

	VertexShader(
		Device& device,
		void* data,
		size_t size,
		InputLayoutUniquePtr inputLayout
		);

	void bind(Device& device) const override;

private:

	mutable system::COMWrapper<ID3D11VertexShader> shader_; // because PSSetShader takes PixelShader* not const PixelShader*

	InputLayoutUniquePtr inputLayout_; // if it stays here, just use dx type and provide description in constructor (?)

};

MAKE_POINTER_DEFINITIONS(VertexShader);

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_VERTEXSHADER_HPP_ */
