#include "ShaderResourceView.hpp"

#include "DirectXError.hpp"
#include "Texture.hpp"
#include "Buffer.hpp"
#include "Renderer.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::graphics;

ShaderResourceView::ShaderResourceView(Renderer& renderer, const Texture& texture) {
	checkDirectXCall(
		renderer.internalDevice().CreateShaderResourceView(
			texture.internalResource(),
			nullptr,
			&srv_.get()
			),
		"Failed to create a shader resource view of texture"
		);
}

ShaderResourceView::ShaderResourceView(Renderer& renderer, const Buffer& buffer, PixelFormat elementFormat) {
	auto* bufferInterface = reinterpret_cast<ID3D11Buffer*>(buffer.internalResource());
	auto bufferDesc = D3D11_BUFFER_DESC();
	bufferInterface->GetDesc(&bufferDesc);

	auto desc = D3D11_SHADER_RESOURCE_VIEW_DESC();
	std::memset(&desc, 0, sizeof(decltype(desc)));

	desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	desc.Format = static_cast<DXGI_FORMAT>(elementFormat);
	desc.Buffer.ElementOffset = 0;
	desc.Buffer.NumElements = bufferDesc.ByteWidth / formatSize(elementFormat);

	checkDirectXCall(
		renderer.internalDevice().CreateShaderResourceView(bufferInterface, &desc, &srv_.get()),
		"Failed to create a shader resource view of buffer"
	);
}
