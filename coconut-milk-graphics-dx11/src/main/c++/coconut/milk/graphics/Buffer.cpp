#include "Buffer.hpp"

#include <cstring>
#include <stdexcept>
#include <functional>

#include "Renderer.hpp"
#include "DirectXError.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::graphics;

Buffer::Buffer(Renderer& renderer, CreationPurpose purpose, Configuration configuration, const void* initialData) :
	configuration_(std::move(configuration))
{
	D3D11_BUFFER_DESC desc;
	std::memset(&desc, 0, sizeof(desc));

	desc.ByteWidth = static_cast<UINT>(configuration_.size);
	desc.BindFlags = static_cast<UINT>(purpose);
	desc.StructureByteStride = static_cast<UINT>(configuration_.stride);

	if (configuration_.allowModifications) {
		if (configuration_.allowCPURead) {
			desc.Usage = D3D11_USAGE_STAGING;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
		} else {
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}
	} else {
		if (configuration_.allowCPURead) {
			desc.Usage = D3D11_USAGE_STAGING;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		} else {
			if (configuration_.allowGPUWrite) {
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
