#ifndef _COCONUT_MILK_GRAPHICS_VERTEXSHADER_HPP_
#define _COCONUT_MILK_GRAPHICS_VERTEXSHADER_HPP_

#include <memory>
#include <vector>

#include <d3d11.h>

#include "Buffer.hpp"
#include "ShaderParametersDescription.hpp"
#include "milk/system/COMWrapper.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class Device;
class InputLayoutDescription;
class Renderable;

class VertexShader {
public:

	VertexShader(
		Device& device,
		void* data,
		size_t size,
		InputLayoutDescription& inputLayout,
		std::shared_ptr<ShaderParametersDescription> shaderParameters
		);

	void update(Device& device, const Renderable& renderable);

	void bind(Device& device, const Renderable& renderable);

private:

	system::COMWrapper<ID3D11VertexShader> shader_;

	system::COMWrapper<ID3D11InputLayout> inputLayout_;

	std::shared_ptr<ShaderParametersDescription> shaderParameters_;

};

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_VERTEXSHADER_HPP_ */
