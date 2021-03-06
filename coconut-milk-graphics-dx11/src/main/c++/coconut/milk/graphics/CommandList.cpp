#include "CommandList.hpp"

#include <coconut-tools/exceptions/LogicError.hpp>

#include "ConstantBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexBuffer.hpp"
#include "Renderer.hpp"
#include "RenderState.hpp"
#include "Resource.hpp"
#include "Sampler.hpp"
#include "DirectXError.hpp"
#include "ShaderType.hpp"
#include "PixelFormat.hpp"
#include "InputLayout.hpp"
#include "Viewport.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::graphics;

CommandList::CommandList() {
}

CommandList::CommandList(system::COMWrapper<ID3D11DeviceContext> internalDeviceContext) :
	deviceContext_(std::move(internalDeviceContext))
{
}

void CommandList::initialise(system::COMWrapper<ID3D11DeviceContext> internalDeviceContext) {
	deviceContext_ = internalDeviceContext;
}

void CommandList::drawIndexed(size_t startingIndex, size_t indexCount, PrimitiveTopology primitiveTopology) {
	deviceContext_->IASetPrimitiveTopology(static_cast<D3D11_PRIMITIVE_TOPOLOGY>(primitiveTopology));
	deviceContext_->DrawIndexed(static_cast<UINT>(indexCount), static_cast<UINT>(startingIndex), 0);
}

void CommandList::drawIndexedInstanced(size_t vertexCountPerInstance, size_t instanceCount,
	size_t startingIndex, PrimitiveTopology primitiveTopology)
{
	deviceContext_->IASetPrimitiveTopology(static_cast<D3D11_PRIMITIVE_TOPOLOGY>(primitiveTopology));
	deviceContext_->DrawIndexedInstanced(
		static_cast<UINT>(vertexCountPerInstance),
		static_cast<UINT>(instanceCount),
		static_cast<UINT>(startingIndex),
		0,
		0u
		);
}

CommandList::LockedData CommandList::lock(Resource& data, LockPurpose lockPurpose) {
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	checkDirectXCall(
		deviceContext_->Map(data.internalResource(), 0, static_cast<D3D11_MAP>(lockPurpose), 0, &mappedResource),
		"Failed to map the provided resource"
		);

	return LockedData(
		reinterpret_cast<std::uint8_t*>(mappedResource.pData),
		[deviceContext = deviceContext_, internalBuffer = data.internalResource()](std::uint8_t*) {
			deviceContext->Unmap(internalBuffer, 0);
		}
		);
}

void CommandList::setRenderTarget(Texture2d& renderTarget, Texture2d& depthStencil) {
	auto* renderTargetView = &renderTarget.internalRenderTargetView();
	auto* depthStencilView = &depthStencil.internalDepthStencilView();
	deviceContext_->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
}

void CommandList::setViewport(Viewport& viewport) {
	deviceContext_->RSSetViewports(1, &viewport.internalViewport());
}

void CommandList::setInputLayout(const InputLayout* inputLayout) noexcept {
	deviceContext_->IASetInputLayout(&inputLayout->internalInputLayout());
}

void CommandList::setVertexShader(VertexShader* vertexShader) noexcept {
	deviceContext_->VSSetShader(&vertexShader->internalShader(), nullptr, 0);
}

void CommandList::setGeometryShader(GeometryShader* geometryShader) noexcept {
	if (geometryShader != nullptr) {
		deviceContext_->GSSetShader(&geometryShader->internalShader(), nullptr, 0);
	} else {
		deviceContext_->GSSetShader(nullptr, nullptr, 0);
	}
}

void CommandList::setHullShader(HullShader* hullShader) noexcept {
	if (hullShader != nullptr) {
		deviceContext_->HSSetShader(&hullShader->internalShader(), nullptr, 0);
	}
	else {
		deviceContext_->HSSetShader(nullptr, nullptr, 0);
	}
}

void CommandList::setDomainShader(DomainShader* domainShader) noexcept {
	if (domainShader != nullptr) {
		deviceContext_->DSSetShader(&domainShader->internalShader(), nullptr, 0);
	}
	else {
		deviceContext_->DSSetShader(nullptr, nullptr, 0);
	}
}

void CommandList::setPixelShader(PixelShader* pixelShader) noexcept {
	deviceContext_->PSSetShader(&pixelShader->internalShader(), nullptr, 0);
}

void CommandList::setConstantBuffer(ConstantBuffer& buffer, ShaderType stage, size_t slot) {
	auto* buf = reinterpret_cast<ID3D11Buffer*>(buffer.internalResource().get());

	switch (stage) {
	case ShaderType::VERTEX:
		deviceContext_->VSSetConstantBuffers(static_cast<UINT>(slot), 1, &buf);
		break;
	case ShaderType::GEOMETRY:
		deviceContext_->GSSetConstantBuffers(static_cast<UINT>(slot), 1, &buf);
		break;
	case ShaderType::HULL:
		deviceContext_->HSSetConstantBuffers(static_cast<UINT>(slot), 1, &buf);
		break;
	case ShaderType::DOMAIN:
		deviceContext_->DSSetConstantBuffers(static_cast<UINT>(slot), 1, &buf);
		break;
	case ShaderType::PIXEL:
		deviceContext_->PSSetConstantBuffers(static_cast<UINT>(slot), 1, &buf);
		break;
	default:
		throw coconut_tools::exceptions::LogicError("Unknown shader type: " + toString(stage));
	}
}

void CommandList::setIndexBuffer(IndexBuffer& buffer, size_t offset) {
	auto* buf = reinterpret_cast<ID3D11Buffer*>(buffer.internalResource().get());

	deviceContext_->IASetIndexBuffer(buf, static_cast<DXGI_FORMAT>(buffer.pixelFormat()), static_cast<UINT>(offset));
}

void CommandList::setVertexBuffer(VertexBuffer& buffer, size_t slot) {
	auto strideParam = static_cast<UINT>(buffer.stride());
	UINT offsetParam = 0;
	auto* buf = reinterpret_cast<ID3D11Buffer*>(buffer.internalResource().get());

	deviceContext_->IASetVertexBuffers(static_cast<UINT>(slot), 1, &buf, &strideParam, &offsetParam);
}

void CommandList::setInstanceDataBuffer(VertexBuffer& buffer, size_t slot) {
	auto strideParam = static_cast<UINT>(buffer.stride());
	UINT offsetParam = 0;
	auto* buf = reinterpret_cast<ID3D11Buffer*>(buffer.internalResource().get());

	deviceContext_->IASetVertexBuffers(static_cast<UINT>(slot), 1, &buf, &strideParam, &offsetParam);
}

void CommandList::setResource(const Resource& resource, ShaderType stage, size_t slot) {
	auto* srv = resource.internalShaderResourceView().get();

	switch (stage) {
	case ShaderType::VERTEX:
		deviceContext_->VSSetShaderResources(static_cast<UINT>(slot), 1, &srv);
		break;
	case ShaderType::GEOMETRY:
		deviceContext_->GSSetShaderResources(static_cast<UINT>(slot), 1, &srv);
		break;
	case ShaderType::HULL:
		deviceContext_->HSSetShaderResources(static_cast<UINT>(slot), 1, &srv);
		break;
	case ShaderType::DOMAIN:
		deviceContext_->DSSetShaderResources(static_cast<UINT>(slot), 1, &srv);
		break;
	case ShaderType::PIXEL:
		deviceContext_->PSSetShaderResources(static_cast<UINT>(slot), 1, &srv);
		break;
	default:
		throw coconut_tools::exceptions::LogicError("Unknown shader type: " + toString(stage));
	}
}

void CommandList::setSampler(Sampler& sampler, ShaderType stage, size_t slot) {
	auto* ss = &sampler.internalSamplerState();

	switch (stage) {
	case ShaderType::VERTEX:
		deviceContext_->VSSetSamplers(static_cast<UINT>(slot), 1, &ss);
		break;
	case ShaderType::GEOMETRY:
		deviceContext_->GSSetSamplers(static_cast<UINT>(slot), 1, &ss);
		break;
	case ShaderType::HULL:
		deviceContext_->HSSetSamplers(static_cast<UINT>(slot), 1, &ss);
		break;
	case ShaderType::DOMAIN:
		deviceContext_->DSSetSamplers(static_cast<UINT>(slot), 1, &ss);
		break;
	case ShaderType::PIXEL:
		deviceContext_->PSSetSamplers(static_cast<UINT>(slot), 1, &ss);
		break;
	default:
		throw coconut_tools::exceptions::LogicError("Unknown shader type: " + toString(stage));
	}
}

void CommandList::setRenderState(const RenderState& renderState) {
	deviceContext_->RSSetState(&renderState.internalRasteriserState());
	deviceContext_->OMSetBlendState(&renderState.internalBlendState(), 0, 0xffffffff); // TODO: args
}
