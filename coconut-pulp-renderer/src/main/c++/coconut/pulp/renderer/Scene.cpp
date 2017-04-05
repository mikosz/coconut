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
	actors_.emplace_back(std::move(actor));
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

void Scene::render(CommandBuffer& commandBuffer) {
	PassContext context;
	context.viewport = &viewport_;
	context.backBuffer = renderTarget_;
	context.screenDepthStencil = depthStencil_;
	context.scene = this;

	auto modelToActor = std::unordered_multimap<std::string, Actor*>();
	for (const auto& actor : actors_) {
		modelToActor.emplace(actor->modelId(), actor.get());
	}

	auto actors = std::vector<const Actor*>(); // TODO: can't stay like this

	context.passType = mesh::MaterialConfiguration::PassType::OPAQUE;

	for (const auto& model : models_) { // TODO: this is obviously temp
		auto modelId = std::string();
		auto modelPtr = ModelSharedPtr();
		std::tie(modelId, modelPtr) = model;

		actors.resize(modelToActor.count(modelId));
		auto actorRange = modelToActor.equal_range(modelId);
		std::transform(actorRange.first, actorRange.second, actors.begin(), [](const auto& entry) {
				return entry.second;
			});

		context.model = modelPtr.get();
		context.actors = &actors;

		modelPtr->render(commandBuffer, context);
	}
}
