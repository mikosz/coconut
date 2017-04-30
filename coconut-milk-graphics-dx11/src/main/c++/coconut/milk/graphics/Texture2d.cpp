#include "Texture2d.hpp"

#include <cstring>

#include "Renderer.hpp"
#include "DirectXError.hpp"
#include "ImageLoader.hpp" // TODO: will only need Image when it gets extracted

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::graphics;

Texture2d::Texture2d(Renderer& renderer, const Configuration& configuration) {
	initialise(renderer, configuration);
}

Texture2d::Texture2d(Renderer& renderer, const Image& image) {
	Configuration config;
	config.width = image.size().first;
	config.height = image.size().second;
	// TODO: get following as param
	config.mipLevels = 1;
	config.sampleCount = 1;
	config.sampleQuality = 0;
	config.pixelFormat = image.pixelFormat();
	config.allowModifications = false;
	config.allowCPURead = false;
	config.allowGPUWrite = false;
	config.purposeFlags = static_cast<std::underlying_type_t<CreationPurpose>>(CreationPurpose::SHADER_RESOURCE);
	config.initialData = image.pixels();
	config.dataRowPitch = image.rowPitch();

	initialise(renderer, config);
}

void Texture2d::initialise(Renderer& renderer, const Configuration& configuration) {
	reset();

	D3D11_TEXTURE2D_DESC desc;
	std::memset(&desc, 0, sizeof(desc));

	// TODO: extract common configuration elements to superclass (when implementing other texture types)
	desc.Width = static_cast<UINT>(configuration.width);
	desc.Height = static_cast<UINT>(configuration.height);
	desc.MipLevels = static_cast<UINT>(configuration.mipLevels);
	desc.ArraySize = 1;
	desc.Format = static_cast<DXGI_FORMAT>(configuration.pixelFormat);
	desc.SampleDesc.Count = static_cast<UINT>(configuration.sampleCount);
	desc.SampleDesc.Quality = static_cast<UINT>(configuration.sampleQuality);
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
	D3D11_SUBRESOURCE_DATA* subresourceDataPtr = nullptr;

	if (configuration.initialData) {
		std::memset(&subresourceData, 0, sizeof(subresourceData));

		subresourceData.pSysMem = configuration.initialData;
		subresourceData.SysMemPitch = static_cast<UINT>(configuration.dataRowPitch);

		subresourceDataPtr = &subresourceData;
	}

	ID3D11Texture2D* texture;
	checkDirectXCall(
		renderer.internalDevice().CreateTexture2D(&desc, subresourceDataPtr, &texture),
		"Failed to create a 2D texture"
		);
	texture_.reset(texture); // TODO: wtf???!!! getting a crash when using &texture_.get()

	Texture::initialise(renderer, configuration.purposeFlags);
}

void Texture2d::initialise(
	Renderer& renderer,
	CreationPurposeFlag purposeFlags,
	system::COMWrapper<ID3D11Texture2D> texture
	) {
	reset();

	texture_ = std::move(texture);

	Texture::initialise(renderer, purposeFlags);
}

void Texture2d::reset() {
	texture_.reset();
}
