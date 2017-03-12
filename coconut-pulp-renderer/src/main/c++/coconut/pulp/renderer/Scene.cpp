#include "Scene.hpp"

#include "PassContext.hpp"

#include "shader/PassFactory.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;

namespace /* anonymous */ {

milk::graphics::Viewport::Configuration viewportConfiguration() {
	milk::graphics::Viewport::Configuration configuration; // TODO TODO TODO

	configuration.width = 800.0f;
	configuration.height = 600.0f;
	configuration.minDepth = 0.0f;
	configuration.maxDepth = 1.0f;
	configuration.topLeftX = 0.0f;
	configuration.topLeftY = 0.0f;

	return configuration;
}

} // anonymous namespace

Scene::Scene(milk::graphics::Renderer& graphicsRenderer) :
	renderTarget_(&graphicsRenderer.backBuffer()), // TODO
	depthStencil_(&graphicsRenderer.depthStencil()), // TODO
	viewport_(viewportConfiguration()) // TODO
{
}

void Scene::add(ActorSharedPtr actor) {
	actors_.push_back(actor);
}

void Scene::add(lighting::DirectionalLight directionalLight) {
	directionalLights_.emplace_back(directionalLight);
}

void Scene::add(lighting::PointLight pointLight) {
	pointLights_.emplace_back(pointLight);
}

void Scene::setCamera(CameraSharedPtr camera) {
	camera_ = camera;
}

void Scene::setLens(LensSharedPtr lens) {
	lens_ = lens;
}

void Scene::render(CommandBuffer& commandBuffer) {
	PassContext context;
	context.viewport = &viewport_;
	context.backBuffer = renderTarget_;
	context.screenDepthStencil = depthStencil_;
	context.scene = this;

	context.passType = mesh::MaterialConfiguration::PassType::OPAQUE;

	for (auto actor : actors_) {
		actor->render(commandBuffer, context);
	}

	context.passType = mesh::MaterialConfiguration::PassType::TRANSPARENT;

	for (auto actor : actors_) {
		actor->render(commandBuffer, context);
	}

}
