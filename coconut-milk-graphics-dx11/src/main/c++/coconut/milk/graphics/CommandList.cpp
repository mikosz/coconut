#include "CommandList.hpp"

#include "Renderer.hpp"
#include "DirectXError.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::graphics;

CommandList::CommandList(Renderer& renderer) {
	checkDirectXCall(
		renderer.internalDevice().CreateDeferredContext(0, &d3dDeviceContext_.get()),
		"Failed to create a deferred context"
		);
}

void CommandList::draw(size_t startingIndex, size_t indexCount, PrimitiveTopology primitiveTopology) {
	d3dDeviceContext_->IASetPrimitiveTopology(static_cast<D3D11_PRIMITIVE_TOPOLOGY>(primitiveTopology));
	d3dDeviceContext_->DrawIndexed(static_cast<UINT>(indexCount), static_cast<UINT>(startingIndex), 0);
}

CommandList::LockedData CommandList::lock(Data& data, LockPurpose lockPurpose) {
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	checkDirectXCall(
		d3dDeviceContext_->Map(&data.internalResource(), 0, static_cast<D3D11_MAP>(lockPurpose), 0, &mappedResource),
		"Failed to map the provided resource"
		);

	return Buffer::LockedData(
		mappedResource.pData,
		[deviceContext = d3dDeviceContext_, internalBuffer = data.internalResource()](void*) {
			deviceContext->Unmap(internalBuffer.get(), 0);
		}
		);
}

void CommandList::setRenderTarget(Texture2d& renderTarget, Texture2d& depthStencil) {
	auto* renderTargetView = renderTarget.internalRenderTargetView();
	auto* depthStencilView = depthStencil.internalDepthStencilView();
	d3dDeviceContext_->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
}

void CommandList::setVertexShader(VertexShader& vertexShader) {
	d3dDeviceContext_->VSSetShader(&vertexShader.internalShader(), nullptr, 0);
}

void CommandList::setPixelShader(PixelShader& pixelShader) {
	d3dDeviceContext_->PSSetShader(&pixelShader.internalShader(), nullptr, 0);
}
