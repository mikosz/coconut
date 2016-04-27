#ifndef _COCONUT_PULP_RENDERER_RENDERINGCONTEXT_HPP_
#define _COCONUT_PULP_RENDERER_RENDERINGCONTEXT_HPP_

#include "material/Material.hpp"
#include "shader/Pass.hpp"

#include "coconut/milk/graphics/Texture2d.hpp"
#include "coconut/milk/graphics/Viewport.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class Scene;
class Actor;
class Model;

// TODO: I don't like these pointers
struct RenderingContext {

	milk::graphics::Viewport* viewport = nullptr;

	milk::graphics::Texture2d* backBuffer = nullptr;

	milk::graphics::Texture2d* screenDepthStencil = nullptr;

	shader::Pass* pass = nullptr;

	const Scene* scene = nullptr;

	const Actor* actor = nullptr;

	const Model* model = nullptr;

	const material::Material* material = nullptr;

	void reset() {
		viewport = nullptr;
		backBuffer = nullptr;
		screenDepthStencil = nullptr;
		pass = nullptr;
		scene = nullptr;
		actor = nullptr;
		model = nullptr;
		material = nullptr;
	}

};

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_RENDERINGCONTEXT_HPP_ */
