#include "Texture2d.hpp"

#include <cstring>

#include <d3d11.h>
#include "coconut/milk/system/cleanup-windows-macros.hpp"

#include <coconut-tools/utils/Range.hpp>

#include "Renderer.hpp"
#include "DirectXError.hpp"
#include "ImageLoader.hpp" // TODO: will only need Image when it gets extracted

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::graphics;

namespace /* anonymous */ {

system::COMWrapper<ID3D11Texture2D> createTexture(
	Renderer& renderer,
	const Texture2d::Configuration& configuration
	)
{
	auto desc = D3D11_TEXTURE2D_DESC();
	std::memset(&desc, 0, sizeof(desc));

	// TODO: extract common configuration elements to superclass (when implementing other texture types)
	desc.Width = static_cast<UINT>(configuration.width);
	desc.Height = static_cast<UINT>(configuration.height);
	desc.MipLevels = static_cast<UINT>(configuration.mipLevels);
	desc.ArraySize = static_cast<UINT>(configuration.arraySize);
	desc.Format = static_cast<DXGI_FORMAT>(configuration.pixelFormat);
	desc.SampleDesc.Count = static_cast<UINT>(configuration.sampleCount);
	desc.SampleDesc.Quality = static_cast<UINT>(configuration.sampleQuality);
	desc.BindFlags = configuration.purpose.integralValue();
	desc.MiscFlags = configuration.arraySize == 6 ? D3D11_RESOURCE_MISC_TEXTURECUBE : 0u; // TODO

	// TODO: duplicated with Buffer
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

	auto subresourceData = std::vector<D3D11_SUBRESOURCE_DATA>();
	auto* subresourceDataPtr = static_cast<D3D11_SUBRESOURCE_DATA*>(nullptr);

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

				subresourceData[subresourceIndex].pSysMem = data;
				// TODO: this will not work for compressed formats
				subresourceData[subresourceIndex].SysMemPitch = pixelSize * textureWidth;
				subresourceData[subresourceIndex].SysMemSlicePitch =
					subresourceData[mipIndex].SysMemPitch * textureHeight;

				data += textureWidth * textureHeight * pixelSize;
			}
		}

		subresourceDataPtr = subresourceData.data();
	}

	auto texture = system::COMWrapper<ID3D11Texture2D>();
	checkDirectXCall(
		renderer.internalDevice().CreateTexture2D(&desc, subresourceDataPtr, &texture.get()),
		"Failed to create a 2D texture"
	);

	return texture;
}

Texture2d::Configuration createConfiguration(const Image& image) {
	auto config = Texture2d::Configuration();
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
	config.purpose = Texture2d::CreationPurpose::SHADER_RESOURCE;
	config.initialData = image.pixels();
	config.dataRowPitch = image.rowPitch();

	return config;
}

} // anonymous namespace

Texture2d::Texture2d(Renderer& renderer, const Configuration& configuration) :
	Texture(createTexture(renderer, configuration))
{
}

Texture2d::Texture2d(Renderer& renderer, const Image& image) :
	Texture2d(renderer, createConfiguration(image))
{
}

RenderTargetView::RenderTargetView(Renderer& renderer, const Texture2d& texture) {
	checkDirectXCall(
		renderer.internalDevice().CreateRenderTargetView(
			texture.internalResource(),
			nullptr,
			&rtv_.get()
			),
		"Failed to create a render target view of texture"
		);
}

DepthStencilView::DepthStencilView(Renderer& renderer, const Texture2d& texture) {
	checkDirectXCall(
		renderer.internalDevice().CreateDepthStencilView(
			texture.internalResource(),
			nullptr,
			&dsv_.get()
			),
		"Failed to create a depth stencil view of texture"
		);
}

DepthStencilView::DepthStencilView(
	Renderer& renderer,
	const Texture2d& texture,
	Configuration configuration
	)
{
	auto desc = D3D11_DEPTH_STENCIL_VIEW_DESC();
	std::memset(&desc, 0, sizeof(desc));

	desc.Format = static_cast<DXGI_FORMAT>(configuration.pixelFormat);
	desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipSlice = configuration.mipSlice;

	checkDirectXCall(
		renderer.internalDevice().CreateDepthStencilView(
			texture.internalResource(),
			&desc,
			&dsv_.get()
			),
		"Failed to create a depth stencil view of texture"
		);
}
