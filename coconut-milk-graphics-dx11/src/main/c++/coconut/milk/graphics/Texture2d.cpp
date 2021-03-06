#include "Texture2d.hpp"

#include <cstring>

#include <coconut-tools/utils/Range.hpp>

#include "Renderer.hpp"
#include "DirectXError.hpp"
#include "ImageLoader.hpp" // TODO: will only need Image when it gets extracted

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::graphics;

// TODO: this whole setup with Resource/Buffer/Texture/Texture1d/Texture2d is messy and
// unnecessary

Texture2d::Texture2d(Renderer& renderer, const Configuration& configuration) {
	initialise(renderer, configuration);
}

Texture2d::Texture2d(Renderer& renderer, const Image& image) {
	Configuration config;
	config.width = image.size().first;
	config.height = image.size().second;
	config.arraySize = image.arraySize();
	config.mipLevels = image.mipLevels();
	config.sampleCount = 1;
	config.sampleQuality = 0;
	config.pixelFormat = image.pixelFormat();
	config.allowModifications = false;
	config.allowCPURead = false;
	config.allowGPUWrite = false;
	config.purposeFlags = CreationPurpose::SHADER_RESOURCE;
	config.initialData = image.pixels();

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
	desc.ArraySize = static_cast<UINT>(configuration.arraySize);
	desc.Format = static_cast<DXGI_FORMAT>(configuration.pixelFormat);
	desc.SampleDesc.Count = static_cast<UINT>(configuration.sampleCount);
	desc.SampleDesc.Quality = static_cast<UINT>(configuration.sampleQuality);
	desc.BindFlags = configuration.purposeFlags.integralValue();
	desc.MiscFlags = configuration.arraySize == 6 ? D3D11_RESOURCE_MISC_TEXTURECUBE : 0u; // TODO

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

	std::vector<D3D11_SUBRESOURCE_DATA> subresourceData;
	D3D11_SUBRESOURCE_DATA* subresourceDataPtr = nullptr;

	if (configuration.initialData) {
		subresourceData.resize(configuration.arraySize * configuration.mipLevels);
		std::memset(subresourceData.data(), 0, subresourceData.size() * sizeof(D3D11_SUBRESOURCE_DATA));

		const auto pixelSize = formatSize(configuration.pixelFormat);
		const auto* data = reinterpret_cast<const std::uint8_t*>(configuration.initialData);

		for (const auto textureIndex : coconut_tools::range(size_t(0), configuration.arraySize)) {
			for (const auto mipIndex : coconut_tools::range(size_t(0), configuration.mipLevels)) {
				const auto subresourceIndex = textureIndex * configuration.mipLevels + mipIndex;
				const auto textureWidth = configuration.width >> mipIndex;
				const auto textureHeight = configuration.height >> mipIndex;
				const auto rowPitch = formatRowPitch(configuration.pixelFormat, textureWidth);
				const auto slicePitch = formatSlicePitch(configuration.pixelFormat, textureHeight, rowPitch);

				subresourceData[subresourceIndex].pSysMem = data;
				// TODO: this will not work for compressed formats
				subresourceData[subresourceIndex].SysMemPitch = static_cast<UINT>(rowPitch);
				subresourceData[subresourceIndex].SysMemSlicePitch = static_cast<UINT>(slicePitch);

				data += slicePitch;
			}
		}

		subresourceDataPtr = subresourceData.data();
	}

	auto texture = system::COMWrapper<ID3D11Texture2D>();
	checkDirectXCall(
		renderer.internalDevice().CreateTexture2D(&desc, subresourceDataPtr, &texture.get()),
		"Failed to create a 2D texture"
		);
	resource_ = std::move(texture);

	Texture::initialise(renderer, configuration.purposeFlags);
}

void Texture2d::initialise(
    Renderer& renderer,
    coconut_tools::Mask<CreationPurpose> purposeFlags,
    system::COMWrapper<ID3D11Texture2D> texture
    )
{
	reset();

	resource_ = std::move(texture);

	Texture::initialise(renderer, purposeFlags);
}
