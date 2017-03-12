#ifndef _COCONUT_PULP_RENDERER_SCENE_HPP_
#define _COCONUT_PULP_RENDERER_SCENE_HPP_

#include <vector>

#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

#include "coconut/milk/graphics/Renderer.hpp"
#include "coconut/milk/graphics/Viewport.hpp"

#include "shader/Pass.hpp"
#include "lighting/DirectionalLight.hpp"
#include "lighting/PointLight.hpp"

#include "Actor.hpp"
#include "Camera.hpp"
#include "Lens.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class CommandBuffer;

class Scene {
public:

	Scene(milk::graphics::Renderer& graphicsRenderer);

	void add(ActorSharedPtr actor);

	void add(lighting::DirectionalLight directionalLight);

	void add(lighting::PointLight pointLight);

	void setCamera(CameraSharedPtr camera);

	void setLens(LensSharedPtr lens);

	void render(CommandBuffer& commandBuffer);

	const Camera& camera() const {
		return *camera_;
	}

	const Lens& lens() const {
		return *lens_;
	}

	const std::vector<lighting::DirectionalLight>& directionalLights() const {
		return directionalLights_;
	}

	const std::vector<lighting::PointLight>& pointLights() const {
		return pointLights_;
	}

private:

	std::vector<ActorSharedPtr> actors_;

	std::vector<lighting::DirectionalLight> directionalLights_;

	std::vector<lighting::PointLight> pointLights_;

	CameraSharedPtr camera_;

	LensSharedPtr lens_;

	milk::graphics::Texture2d* renderTarget_; // TODO

	milk::graphics::Texture2d* depthStencil_; // TODO

	milk::graphics::Viewport viewport_; // TODO

};

CCN_MAKE_POINTER_DEFINITIONS(Scene);

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SCENE_HPP_ */
