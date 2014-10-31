#include "Texture2d.hpp"

#include <cstring>
#include <stdexcept>

#include "Device.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::graphics;

Texture2d::Texture2d(Device& device, const Configuration& configuration, void* initialData) {
	D3D11_TEXTURE2D_DESC desc;
	std::memset(&desc, 0, sizeof(desc));

	desc.Width = configuration.width;
	desc.Height = configuration.height;
	desc.MipLevels = configuration.mipLevels;
	desc.ArraySize = 1;
	desc.Format = static_cast<DXGI_FORMAT>(configuration.pixelFormat);
	desc.SampleDesc.Count = 1;
	desc.BindFlags = static_cast<UINT>(configuration.purposeFlags);

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

	D3D11_SUBRESOURCE_DATA subresourceData;
	std::memset(&subresourceData, 0, sizeof(subresourceData));
	
	subresourceData.pSysMem = initialData;

	if (FAILED(device.d3dDevice()->CreateTexture2D(&desc, &subresourceData, &texture_.get()))) {
		throw std::runtime_error("Failed to create a 2D texture");
	}
}

void* Texture2d::lock(Device& device, LockPurpose lockPurpose) {
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	if (FAILED(device.d3dDeviceContext()->Map(texture_, 0, static_cast<D3D11_MAP>(lockPurpose), 0, &mappedResource))) {
		throw std::runtime_error("Failed to map resource");
	}
	
	return mappedResource.pData;
}

void Texture2d::unlock(Device& device) {
	device.d3dDeviceContext()->Unmap(texture_, 0);
}

ID3D11RenderTargetView* Texture2d::asRenderTargetView(Device& device) {
	if (renderTargetView_.get() == 0) {
		device.d3dDevice()->CreateRenderTargetView(texture_, 0, &renderTargetView_.get());
	}

	return renderTargetView_;
}
