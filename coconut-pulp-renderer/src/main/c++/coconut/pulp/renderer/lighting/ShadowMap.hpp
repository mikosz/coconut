#ifndef _COCONUT_PULP_RENDERER_LIGHTING_SHADOWMAP_HPP_
#define _COCONUT_PULP_RENDERER_LIGHTING_SHADOWMAP_HPP_

#include "coconut/milk/graphics/Renderer.hpp"
#include "coconut/milk/graphics/Texture2d.hpp"
#include "coconut/milk/graphics/Viewport.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace lighting {

class ShadowMap {
public:

	ShadowMap(milk::graphics::Renderer& graphicsRenderer, size_t width, size_t height);

private:

	size_t width_;

	size_t height_;

	milk::graphics::Texture2d depthTexture_;

	milk::graphics::Viewport viewport_;

};

} // namespace lighting
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_LIGHTING_SHADOWMAP_HPP_ */
