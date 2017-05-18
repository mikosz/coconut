#include "Buffer.hpp"

#include <cstring>
#include <stdexcept>
#include <functional>

#include "Renderer.hpp"
#include "DirectXError.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::graphics;

Buffer::Buffer(Renderer& renderer, const Configuration& configuration, CreationPurpose purpose) :
	Resource(createResource(renderer, configuration, purpose))
{
}

size_t Buffer::size() const noexcept {
	auto desc = D3D11_BUFFER_DESC();
	internalResource()->GetDesc(&desc);
	return desc.ByteWidth;
}

system::COMWrapper<ID3D11Resource> Buffer::createResource(
	Renderer& renderer,
	const Configuration& configuration,
	CreationPurpose purpose
)
{
	auto desc = D3D11_BUFFER_DESC();
	std::memset(&desc, 0, sizeof(desc));

	desc.ByteWidth = static_cast<UINT>(configuration.size);
	desc.BindFlags = static_cast<UINT>(purpose);
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
	if (configuration.initialData) {
		std::memset(&data, 0, sizeof(data));
		data.pSysMem = configuration.initialData;
		dataPtr = &data;
	}

	auto buffer = system::COMWrapper<ID3D11Buffer>();

	checkDirectXCall(renderer.internalDevice().CreateBuffer(&desc, dataPtr, &buffer.get()),
		"Failed to create a buffer");

	return buffer;
}
