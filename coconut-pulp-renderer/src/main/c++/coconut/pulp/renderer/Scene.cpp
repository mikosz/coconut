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

void Scene::add(ActorSharedPtr actor, ModelSharedPtr model) {
	auto it = instances_.emplace(model->id(), Instance(model)).first;
	auto& instance = it->second;
	instance.actors.emplace_back(std::move(actor));
}

void Scene::add(lighting::DirectionalLight directionalLight) {
	directionalLights_.emplace_back(std::move(directionalLight));
}

void Scene::add(lighting::PointLight pointLight) {
	pointLights_.emplace_back(std::move(pointLight));
}

void Scene::setCamera(CameraSharedPtr camera) {
	camera_ = std::move(camera);
}

void Scene::setLens(LensSharedPtr lens) {
	lens_ = std::move(lens);
}

void Scene::render(PassContext passContext, CommandBuffer& commandBuffer) {
	passContext.viewport = &viewport_;
	passContext.backBuffer = renderTarget_;
	passContext.screenDepthStencil = depthStencil_;
	passContext.scene = this;

	passContext.passType = mesh::MaterialConfiguration::PassType::OPAQUE;

	for (const auto& instanceEntry : instances_) { // TODO: this is obviously temp
		const auto& instance = instanceEntry.second;
		passContext.model = instance.model.get();
		passContext.actors = &instance.actors;

		assert(instance.actors.size() == 1); // ... because instancing is now broken
		auto properties = shader::Properties();
		instance.actors[0]->bindShaderProperties(properties, "actor");

		instance.model->render(commandBuffer, passContext);
	}
}
