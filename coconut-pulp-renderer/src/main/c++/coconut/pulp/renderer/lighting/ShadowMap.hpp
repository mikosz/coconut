#ifndef _COCONUT_PULP_RENDERER_LIGHTING_SHADOWMAP_HPP_
#define _COCONUT_PULP_RENDERER_LIGHTING_SHADOWMAP_HPP_

#include "coconut/milk/graphics/Renderer.hpp"
#include "coconut/milk/graphics/Viewport.hpp"
#include "coconut/milk/graphics/Texture2d.hpp"
#include "coconut/milk/graphics/ShaderResourceView.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace lighting {

class ShadowMap {
public:

	ShadowMap(milk::graphics::Renderer& graphicsRenderer, size_t width, size_t height);

	const milk::graphics::DepthStencilView& depthDSV() const noexcept {
		return depthTextureDSV_;
	}

	const milk::graphics::ShaderResourceView& depthSRV() const noexcept {
		return depthTextureSRV_;
	}

	const milk::graphics::Viewport& viewport() const noexcept {
		return viewport_;
	}

private:

	size_t width_;

	size_t height_;

	milk::graphics::DepthStencilView depthTextureDSV_;

	milk::graphics::ShaderResourceView depthTextureSRV_;

	milk::graphics::Viewport viewport_;

};

} // namespace lighting
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_LIGHTING_SHADOWMAP_HPP_ */
