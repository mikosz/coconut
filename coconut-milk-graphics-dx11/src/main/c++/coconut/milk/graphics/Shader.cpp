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
system::COMWrapper<ID3D11GeometryShader> create<ID3D11GeometryShader>(
	Renderer& renderer, void* data, size_t size) {
	system::COMWrapper<ID3D11GeometryShader> shader;
	checkDirectXCall(
		renderer.internalDevice().CreateGeometryShader(data, size, 0, &shader.get()),
		"Failed to create a geometry shader"
	);
	return shader;
}

template <>
system::COMWrapper<ID3D11HullShader> create<ID3D11HullShader>(
	Renderer& renderer, void* data, size_t size) {
	system::COMWrapper<ID3D11HullShader> shader;
	checkDirectXCall(
		renderer.internalDevice().CreateHullShader(data, size, 0, &shader.get()),
		"Failed to create a hull shader"
	);
	return shader;
}

template <>
system::COMWrapper<ID3D11DomainShader> create<ID3D11DomainShader>(
	Renderer& renderer, void* data, size_t size) {
	system::COMWrapper<ID3D11DomainShader> shader;
	checkDirectXCall(
		renderer.internalDevice().CreateDomainShader(data, size, 0, &shader.get()),
		"Failed to create a domain shader"
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

template <class InternalShaderType>
detail::Shader<InternalShaderType>::Shader(Renderer& renderer, const void* data, size_t size) {
	shader_ = create<InternalShaderType>(renderer, const_cast<void*>(data), size);
}

template class detail::Shader<ID3D11VertexShader>;
template class detail::Shader<ID3D11GeometryShader>;
template class detail::Shader<ID3D11HullShader>;
template class detail::Shader<ID3D11DomainShader>;
template class detail::Shader<ID3D11PixelShader>;
