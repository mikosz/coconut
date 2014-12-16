#ifndef _COCONUT_MILK_GRAPHICS_VERTEXSHADER_HPP_
#define _COCONUT_MILK_GRAPHICS_VERTEXSHADER_HPP_

#include <memory>
#include <vector>

#include <d3d11.h>

#include "milk/system/COMWrapper.hpp"
#include "Buffer.hpp"
#include "Shader.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class Device;
class InputLayoutDescription;

class VertexShader : public Shader {
public:

	VertexShader(
		Device& device,
		void* data,
		size_t size,
		InputLayoutDescription& inputLayout
		);

	void bind(Device& device) override;

private:

	system::COMWrapper<ID3D11VertexShader> shader_;

	system::COMWrapper<ID3D11InputLayout> inputLayout_;

};

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_VERTEXSHADER_HPP_ */
