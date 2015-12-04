#include "Texture2d.hpp"

#include <cstring>
#include <stdexcept>

#include "Device.hpp"
#include "DirectXError.hpp"
#include "ImageLoader.hpp" // TODO: will only nead Image when it gets extracted

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::graphics;

Texture2d::Texture2d(Device& device, const Configuration& configuration, const void* initialData, size_t dataRowPitch) {
	initialise(device, configuration, initialData, dataRowPitch);
}

Texture2d::Texture2d(Device& device, const Image& image) {
	Configuration config;
	config.width = image.size().first;
	config.height = image.size().second;
	// TODO: get following as param
	config.mipLevels = 1;
	config.pixelFormat = image.pixelFormat();
	config.allowModifications = false;
	config.allowCPURead = false;
	config.allowGPUWrite = false;
	config.purposeFlags = CreationPurpose::SHADER_RESOURCE;

	initialise(device, config, image.pixels(), image.rowPitch());
}

void* Texture2d::lock(Device& device, LockPurpose lockPurpose) {
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	checkDirectXCall(
		device.d3dDeviceContext()->Map(texture_, 0, static_cast<D3D11_MAP>(lockPurpose), 0, &mappedResource),
		"Failed to map resource"
		);
	
	return mappedResource.pData;
}

void Texture2d::unlock(Device& device) {
	device.d3dDeviceContext()->Unmap(texture_, 0);
}

ID3D11RenderTargetView* Texture2d::asRenderTargetView(Device& device) {
	if (renderTargetView_.get() == nullptr) {
		checkDirectXCall(
			device.d3dDevice()->CreateRenderTargetView(texture_, 0, &renderTargetView_.get()),
			"Failed to create a render target view of texture"
			);
	}

	return renderTargetView_;
}

ShaderResourceUniquePtr Texture2d::asShaderResource(Device& device) const {
	if (shaderResourceView_.get() == nullptr) {
		checkDirectXCall(
			device.d3dDevice()->CreateShaderResourceView(texture_, nullptr, &shaderResourceView_.get()),
			"Failed to create a shader resource view of texture"
			);
	}

	return std::make_unique<ShaderResource>(shaderResourceView_);
}

void Texture2d::initialise(Device& device, const Configuration& configuration, const void* initialData, size_t dataRowPitch) {
	D3D11_TEXTURE2D_DESC desc;
	std::memset(&desc, 0, sizeof(desc));

	desc.Width = static_cast<UINT>(configuration.width);
	desc.Height = static_cast<UINT>(configuration.height);
	desc.MipLevels = static_cast<UINT>(configuration.mipLevels);
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
	subresourceData.SysMemPitch = static_cast<UINT>(dataRowPitch);

	checkDirectXCall(
		device.d3dDevice()->CreateTexture2D(&desc, &subresourceData, &texture_.get()),
		"Failed to create a 2D texture"
		);
}
