#include "Texture.hpp"

#include "Renderer.hpp"
#include "DirectXError.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::graphics;

void Texture::initialise(Renderer& renderer, std::underlying_type_t<CreationPurpose> purposeFlags) {
	if (purposeFlags & static_cast<std::underlying_type_t<CreationPurpose>>(CreationPurpose::SHADER_RESOURCE)) {
		checkDirectXCall(
			renderer.internalDevice().CreateShaderResourceView(&internalResource(), nullptr, &shaderResourceView_.get()),
			"Failed to create a shader resource view of texture"
			);
	}

	if (purposeFlags & static_cast<std::underlying_type_t<CreationPurpose>>(CreationPurpose::RENDER_TARGET)) {
		checkDirectXCall(
			renderer.internalDevice().CreateRenderTargetView(&internalResource(), nullptr, &renderTargetView_.get()),
			"Failed to create a render target view of texture"
			);
	}

	if (purposeFlags & static_cast<std::underlying_type_t<CreationPurpose>>(CreationPurpose::DEPTH_STENCIL)) {
		checkDirectXCall(
			renderer.internalDevice().CreateDepthStencilView(&internalResource(), nullptr, &depthStencilView_.get()),
			"Failed to create a depth stencil view of texture"
			);
	}
}

void Texture::reset() {
	shaderResourceView_.reset();
	depthStencilView_.reset();
	renderTargetView_.reset();
}
