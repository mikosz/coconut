#include "Buffer.hpp"

#include <cstring>
#include <stdexcept>
#include <functional>

#include "Renderer.hpp"
#include "DirectXError.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::graphics;

Buffer::Buffer(Renderer& renderer, const Configuration& configuration, const void* initialData) :
	configuration_(configuration)
{
	D3D11_BUFFER_DESC desc;
	std::memset(&desc, 0, sizeof(desc));

	desc.ByteWidth = static_cast<UINT>(configuration.size);
	desc.BindFlags = static_cast<UINT>(configuration.purpose);
	desc.StructureByteStride = static_cast<UINT>(configuration.stride);

	if (configuration.allowModifications) {
		if (configuration.allowCPURead) {
			desc.Usage = D3D11_USAGE_STAGING;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
		} else {
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}
	} else {
		if (configuration.allowCPURead) {
			desc.Usage = D3D11_USAGE_STAGING;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		} else {
			if (configuration.allowGPUWrite) {
				desc.Usage = D3D11_USAGE_DEFAULT;
			} else {
				desc.Usage = D3D11_USAGE_IMMUTABLE;
			}
		}
	}

	D3D11_SUBRESOURCE_DATA* dataPtr = 0;
	D3D11_SUBRESOURCE_DATA data;
	if (initialData) {
		std::memset(&data, 0, sizeof(data));
		data.pSysMem = initialData;
		dataPtr = &data;
	}

	checkDirectXCall(renderer.internalDevice().CreateBuffer(&desc, dataPtr, &buffer_.get()),
		"Failed to create a buffer");
}

Buffer::LockedData Buffer::lock(Renderer& renderer, LockPurpose lockPurpose) {
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	checkDirectXCall(
		renderer.internalDeviceContext().Map(buffer_, 0, static_cast<D3D11_MAP>(lockPurpose), 0, &mappedResource),
		"Failed to lock a buffer"
		);

	return Buffer::LockedData(
		mappedResource.pData,
		[&deviceContext = renderer.internalDeviceContext(), resource = buffer_.get()](void*) {
			deviceContext.Unmap(resource, 0);
		}
		);
}

void Buffer::bind(Renderer& renderer, ShaderType shaderType, size_t slot) {
	ID3D11Buffer* buffer = buffer_.get();

	switch (configuration_.purpose) {
	case CreationPurpose::VERTEX_BUFFER:
		{
			UINT stride = static_cast<UINT>(configuration_.stride);
			UINT offset = 0;
			renderer.internalDeviceContext().IASetVertexBuffers(static_cast<UINT>(slot), 1, &buffer, &stride, &offset);
			break;
		}
	case CreationPurpose::INDEX_BUFFER:
		{
			DXGI_FORMAT format;
			switch (configuration_.stride) {
			case 2:
				format = DXGI_FORMAT_R16_UINT;
				break;
			case 4:
				format = DXGI_FORMAT_R32_UINT;
				break;
			default:
				throw std::runtime_error("Invalid index buffer stride. Allowed strides are 2B and 4B.");
			}
			renderer.internalDeviceContext().IASetIndexBuffer(buffer, format, 0);
			break;
		}
	case CreationPurpose::CONSTANT_BUFFER:
		{
			switch (shaderType) {
			case ShaderType::VERTEX:
				renderer.internalDeviceContext().VSSetConstantBuffers(static_cast<UINT>(slot), 1, &buffer);
				break;
			case ShaderType::PIXEL:
				renderer.internalDeviceContext().PSSetConstantBuffers(static_cast<UINT>(slot), 1, &buffer);
				break;
			default:
				throw std::runtime_error("Unknown shader type");
			}

			break;
		}
	default:
		throw std::runtime_error("Unknown buffer purpose");
	}
}
