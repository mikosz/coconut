#ifndef _COCONUT_PULP_RENDERER_RENDERINGCONTEXT_HPP_
#define _COCONUT_PULP_RENDERER_RENDERINGCONTEXT_HPP_

namespace coconut {
namespace pulp {
namespace renderer {

class Scene;
class Actor;
class Model;
class Material;

struct RenderingContext {

	Scene* scene;

	Actor* actor;

	Model* model;

	Material* material;

	RenderingContext() :
		scene(nullptr),
		actor(nullptr),
		model(nullptr),
		material(nullptr)
	{
	}

};

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_RENDERINGCONTEXT_HPP_ */
