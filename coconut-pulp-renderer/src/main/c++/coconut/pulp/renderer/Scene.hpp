#ifndef _COCONUT_PULP_RENDERER_SCENE_HPP_
#define _COCONUT_PULP_RENDERER_SCENE_HPP_

#include <vector>

#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"
#include "coconut/milk/graphics/Device.hpp"

#include "shader/Pass.hpp"

#include "Actor.hpp"
#include "Camera.hpp"
#include "Lens.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class Scene {
public:

	Scene(milk::graphics::Device& device);

	void add(ActorSharedPtr actor);

	void setCamera(CameraSharedPtr camera);

	void setLens(LensSharedPtr lens);

	void render(milk::graphics::Device& device);

	const Camera& camera() const {
		return *camera_;
	}

	const Lens& lens() const {
		return *lens_;
	}

	const shader::Pass& renderingPass() const {
		return *renderingPass_;
	}

private:

	std::vector<ActorSharedPtr> actors_;

	CameraSharedPtr camera_;

	LensSharedPtr lens_;

	shader::PassUniquePtr renderingPass_;

};

MAKE_POINTER_DEFINITIONS(Scene);

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SCENE_HPP_ */
