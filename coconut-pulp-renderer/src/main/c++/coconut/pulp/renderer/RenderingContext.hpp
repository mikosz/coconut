#ifndef _COCONUT_PULP_RENDERER_RENDERINGCONTEXT_HPP_
#define _COCONUT_PULP_RENDERER_RENDERINGCONTEXT_HPP_

#include "material/Material.hpp"
#include "shader/Pass.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class Scene;
class Actor;
class Model;

struct RenderingContext {

	const shader::Pass* pass = nullptr;

	const Scene* scene = nullptr;

	const Actor* actor = nullptr;

	const Model* model = nullptr;

	const material::Material* material = nullptr;

	void reset() {
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
