#include "Scene.hpp"

#include "RenderingContext.hpp"

#include "shader/ShaderFactory.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;

Scene::Scene(milk::graphics::Device& device) :
	renderingPass_(std::move(shader::ShaderFactory().createShader(device, "")))
{
	directionalLights_.emplace_back(
		milk::math::Vector3d(-0.5f, -0.5f, -0.5f),
		milk::math::Vector4d(0.1f, 0.1f, 0.1f, 0.0f),
		milk::math::Vector4d(1.0f, 1.0f, 1.0f, 1.0f),
		milk::math::Vector4d(1.0f, 1.0f, 1.0f, 1.0f)
		);
}

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
	context.pass = renderingPass_.get();

	for (auto actor : actors_) {
		actor->render(device, context);
	}
}
