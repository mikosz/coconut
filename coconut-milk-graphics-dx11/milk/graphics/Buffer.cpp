#include "Buffer.hpp"

#include <cstring>
#include <stdexcept>

#include "Device.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::graphics;

Buffer::Buffer(Device& device, const Configuration& configuration, void* initialData) :
	configuration_(configuration)
{
	D3D11_BUFFER_DESC desc;
	std::memset(&desc, 0, sizeof(desc));

	desc.ByteWidth = configuration.size;
	desc.BindFlags = static_cast<UINT>(configuration.purpose);
	desc.StructureByteStride = configuration.stride;

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

	D3D11_SUBRESOURCE_DATA data;
	std::memset(&data, 0, sizeof(data));

	data.pSysMem = initialData;

	if (FAILED(device.d3dDevice()->CreateBuffer(&desc, &data, &buffer_.get()))) {
		throw std::runtime_error("Failed to create a buffer");
	}
}

void* Buffer::lock(Device& device, LockPurpose lockPurpose) {
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	if (FAILED(
		device.d3dDeviceContext()->Map(buffer_, 0, static_cast<D3D11_MAP>(lockPurpose), 0, &mappedResource))
		) {
		throw std::runtime_error("Failed to lock a buffer");
	}

	return mappedResource.pData;
}

void Buffer::unlock(Device& device) {
	device.d3dDeviceContext()->Unmap(buffer_, 0);
}

void Buffer::bind(Device& device, ShaderType shaderType, size_t slot) {
	ID3D11Buffer* buffer = *buffer_;

	switch (configuration_.purpose) {
	case Buffer::VERTEX_BUFFER:
		{
			UINT stride = configuration_.stride;
			UINT offset = 0;
			device.d3dDeviceContext()->IASetVertexBuffers(slot, 1, &buffer, &stride, &offset);
			break;
		}
	case Buffer::INDEX_BUFFER:
		device.d3dDeviceContext()->IASetIndexBuffer(buffer, DXGI_FORMAT_R16_UINT, 0);
		break;
	case Buffer::CONSTANT_BUFFER:
		{
			switch (shaderType) {
			case VERTEX_SHADER:
				device.d3dDeviceContext()->VSSetConstantBuffers(slot, 1, &buffer);
				break;
			case PIXEL_SHADER:
				device.d3dDeviceContext()->PSGetConstantBuffers(slot, 1, &buffer);
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
