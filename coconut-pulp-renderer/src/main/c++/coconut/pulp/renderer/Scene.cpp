#include "Scene.hpp"

#include "RenderingContext.hpp"

#include "shader/ShaderFactory.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;

Scene::Scene(milk::graphics::Device& device) :
	renderingPass_(std::move(shader::ShaderFactory().createShader(device, "")))
{
}

void Scene::add(ActorSharedPtr actor) {
	actors_.push_back(actor);
}

void Scene::add(lighting::DirectionalLight directionalLight) {
	directionalLights_.emplace_back(directionalLight);
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
	context.pass = renderingPass_.get();

	for (auto actor : actors_) {
		actor->render(device, context);
	}
}
