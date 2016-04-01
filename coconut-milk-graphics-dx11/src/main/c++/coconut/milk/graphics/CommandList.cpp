#include "CommandList.hpp"

#include <coconut-tools/exceptions/LogicError.hpp>

#include "Buffer.hpp"
#include "Renderer.hpp"
#include "Rasteriser.hpp"
#include "Texture2d.hpp"
#include "Sampler.hpp"
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
		[deviceContext = d3dDeviceContext_, &internalBuffer = data.internalResource()](void*) {
			deviceContext->Unmap(&internalBuffer, 0);
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

void CommandList::setConstantBuffer(ConstantBuffer& buffer, ShaderType stage, size_t slot) {
	auto* buf = &buffer.internalBuffer();

	switch (stage) {
	case ShaderType::VERTEX:
		d3dDeviceContext_->VSSetConstantBuffers(slot, 1, &buf);
		break;
	case ShaderType::PIXEL:
		d3dDeviceContext_->PSSetConstantBuffers(slot, 1, &buf);
		break;
	default:
		throw coconut_tools::exceptions::LogicError("Unknown shader type: " + toString(stage));
	}
}

void CommandList::setIndexBuffer(IndexBuffer& buffer, size_t offset) {
	auto offsetParam = static_cast<UINT>(offset);
	auto* buf = &buffer.internalBuffer();

	d3dDeviceContext_->IASetIndexBuffer(buf, format, offset);
}

void CommandList::setVertexBuffer(VertexBuffer& buffer, size_t slot, size_t stride) {
	auto strideParam = static_cast<UINT>(stride);
	UINT offsetParam = 0;
	auto* buf = &buffer.internalBuffer();

	d3dDeviceContext_->IASetVertexBuffers(slot, 1, &buf, &strideParam, &offsetParam);
}

void CommandList::setTexture(Texture& texture, ShaderType stage, size_t slot) {
	auto* srv = &texture.internalShaderResourceView();

	switch (stage) {
	case ShaderType::VERTEX:
		d3dDeviceContext_->VSSetShaderResources(slot, 1, &srv);
		break;
	case ShaderType::PIXEL:
		d3dDeviceContext_->PSSetShaderResources(slot, 1, &srv);
		break;
	default:
		throw coconut_tools::exceptions::LogicError("Unknown shader type: " + toString(stage));
	}
}

void CommandList::setSampler(Sampler& sampler, ShaderType stage, size_t slot) {
	auto* ss = &sampler.internalSamplerState();

	switch (stage) {
	case ShaderType::VERTEX:
		d3dDeviceContext_->VSSetSamplers(slot, 1, &ss);
		break;
	case ShaderType::PIXEL:
		d3dDeviceContext_->PSSetSamplers(slot, 1, &ss);
		break;
	default:
		throw coconut_tools::exceptions::LogicError("Unknown shader type: " + toString(stage));
	}
}

void CommandList::setRasteriser(Rasteriser& rasteriser) {
	d3dDeviceContext_->RSSetState(&rasteriser.internalRasteriserState());
}
