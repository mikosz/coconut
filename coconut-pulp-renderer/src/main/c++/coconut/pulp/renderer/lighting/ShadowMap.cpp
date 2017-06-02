#include "ShadowMap.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;
using namespace coconut::pulp::renderer::lighting;

namespace /* anonymous */ {

milk::graphics::Texture2d::Configuration depthTextureConfiguration(size_t width, size_t height) {
	using namespace coconut_tools::enums; // TODO: need to improve on this

	auto result = milk::graphics::Texture2d::Configuration();
	result.width = width;
	result.height = height;
	result.mipLevels = 1;
	result.arraySize = 1;
	result.pixelFormat = milk::graphics::PixelFormat::R24G8_TYPELESS;
	result.allowCPURead = false;
	result.allowGPUWrite = true;
	result.allowModifications = false;
	result.purpose =
		milk::graphics::Texture::CreationPurpose::DEPTH_STENCIL |
		milk::graphics::Texture::CreationPurpose::SHADER_RESOURCE;

	return result;
}

milk::graphics::Viewport::Configuration viewportConfiguration(size_t width, size_t height) {
	auto result = milk::graphics::Viewport::Configuration();
	result.width = width;
	result.height = height;
	result.minDepth = 0.0f;
	result.maxDepth = 1.0f;
	result.topLeftX = 0.0f;
	result.topLeftY = 0.0f;

	return result;
}

} // anonymous namespace

ShadowMap::ShadowMap(milk::graphics::Renderer& graphicsRenderer, size_t width, size_t height) :
	width_(width),
	height_(height),
	viewport_(viewportConfiguration(width, height))
{
	const auto depthTexture =
		milk::graphics::Texture2d(graphicsRenderer, depthTextureConfiguration(width, height));

	auto dsvConfiguration = milk::graphics::DepthStencilView::Configuration();
	dsvConfiguration.pixelFormat = milk::graphics::PixelFormat::D24_UNORM_S8_UINT;
	depthTextureDSV_ = milk::graphics::DepthStencilView(graphicsRenderer, depthTexture, dsvConfiguration);

	auto srvConfiguration = milk::graphics::ShaderResourceView::Configuration();
	srvConfiguration.pixelFormat = milk::graphics::PixelFormat::R24_UNORM_X8_TYPELESS;
	depthTextureSRV_ = milk::graphics::ShaderResourceView(graphicsRenderer, depthTexture, srvConfiguration);
}
