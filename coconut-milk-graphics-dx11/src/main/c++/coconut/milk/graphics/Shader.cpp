#include "Shader.hpp"

#include "Renderer.hpp"
#include "DirectXError.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::graphics;

namespace /* anonymous */ {

template <class InternalShaderType>
system::COMWrapper<InternalShaderType> create(Renderer& renderer, void* data, size_t size);

template <>
system::COMWrapper<ID3D11VertexShader> create<ID3D11VertexShader>(
		Renderer& renderer, void* data, size_t size) {
	system::COMWrapper<ID3D11VertexShader> shader;
	checkDirectXCall(
		renderer.internalDevice().CreateVertexShader(data, size, 0, &shader.get()),
		"Failed to create a vertex shader"
		);
	return shader;
}

template <>
system::COMWrapper<ID3D11PixelShader> create<ID3D11PixelShader>(
	Renderer& renderer, void* data, size_t size) {
	system::COMWrapper<ID3D11PixelShader> shader;
	checkDirectXCall(
		renderer.internalDevice().CreatePixelShader(data, size, 0, &shader.get()),
		"Failed to create a vertex shader"
		);
	return shader;
}

} // anonymous namespace

template <class InternalShaderType, milk::graphics::ShaderType ShaderTypeEnumValue>
detail::Shader<InternalShaderType, ShaderTypeEnumValue>::Shader(Renderer& renderer, void* data, size_t size) {
	shader_ = create<InternalShaderType>(renderer, data, size);
}

template class detail::Shader<ID3D11VertexShader, milk::graphics::ShaderType::VERTEX>;
template class detail::Shader<ID3D11PixelShader, milk::graphics::ShaderType::PIXEL>;
