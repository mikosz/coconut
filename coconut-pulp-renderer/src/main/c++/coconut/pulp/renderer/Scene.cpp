#include "Scene.hpp"

#include "RenderingContext.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;

void Scene::add(ActorSharedPtr actor) {
	actors_.push_back(actor);
}

void Scene::setCamera(CameraSharedPtr camera) {
	camera_ = camera;
}

void Scene::setLens(LensSharedPtr lens) {
	lens_ = lens;
}

void Scene::render(milk::graphics::Device& device) {
	RenderingContext context;
	context.scene = this;

	for (auto actor : actors_) {
		actor->render(device, context);
	}
}
