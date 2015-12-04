#include "Game.hpp"

#include <fstream>
#include <vector>
#include <cstdint>
#include <memory>
#include <chrono>

#include <boost/filesystem.hpp>

#include "coconut/milk/graphics/FlexibleInputLayoutDescription.hpp"
#include "coconut/milk/graphics/VertexShader.hpp"
#include "coconut/milk/graphics/PixelShader.hpp"

#include "coconut/pulp/renderer/model_loader/ObjModelLoader.hpp"
#include "coconut/pulp/renderer/Model.hpp"
#include "coconut/pulp/renderer/PerspectiveLens.hpp"
#include "coconut/pulp/renderer/OrientedCamera.hpp"
#include "coconut/pulp/renderer/Scene.hpp"
#include "coconut/pulp/renderer/Actor.hpp"

#include "globals.hpp"
#include "coconut/milk/system/Window.hpp"

using namespace coconut;
using namespace coconut::shell;
using namespace coconut::shell::game;

Game::Game(std::shared_ptr<milk::system::App> app) :
	app_(app)
{
	{
		milk::system::Window::Configuration configuration;
		configuration.className = "coconut/milk::graphics::device::Window";
		configuration.fullscreen = false;
		configuration.width = 800;
		configuration.height = 600;
		configuration.title = "Game window";

		window_.reset(new milk::system::Window(configuration, app_));
	}

	{
		milk::graphics::Device::Configuration configuration;
		configuration.debugDevice = DEBUG;
		configuration.vsync = false;

		graphicsDevice_.reset(new milk::graphics::Device(*window_, configuration));
	}
}

void Game::loop() {
	pulp::renderer::OrientedCameraSharedPtr camera(new pulp::renderer::OrientedCamera);

	pulp::renderer::LensSharedPtr lens(new pulp::renderer::PerspectiveLens(milk::math::Handedness::LEFT, 1.0f, 800.0f / 600.0f, 0.001f, 1000.0f));

	pulp::renderer::model_loader::ObjModelLoader::IStreamPtr modelIS(new std::ifstream("data/models/Daniel/craig chemise bleu/craig chemis bleu.obj"));
	if (!modelIS->good()) {
		throw std::runtime_error("Failed to open model file");
	}

	pulp::renderer::model_loader::ObjModelLoader::MaterialFileOpenerPtr opener(new pulp::renderer::model_loader::ObjModelLoader::MaterialFileOpener("data/models/Daniel/craig chemise bleu"));
	pulp::renderer::model_loader::ObjModelLoader loader(std::move(modelIS), opener);

	auto start = std::chrono::steady_clock::now();

	camera->setTranslation(milk::math::Vector3d(0.0f, 0.0f, -1.5f));
	camera->setRotation(milk::math::Vector3d(0.0f, 0.0f, 0.0f));

	pulp::renderer::Scene scene(*graphicsDevice_);

	pulp::renderer::ModelSharedPtr m(new pulp::renderer::Model(*graphicsDevice_, loader, scene.renderingPass().inputLayoutDescription()));

	pulp::renderer::ActorSharedPtr actor(new pulp::renderer::Actor(m));

	scene.add(actor);
	scene.setCamera(camera);
	scene.setLens(lens);

	pulp::renderer::ActorSharedPtr actor2(new pulp::renderer::Actor(m));

	// scene.add(actor2);
	actor2->setRotation(milk::math::Vector3d(0.0f, 0.0f, 0.0f));
	actor2->setTranslation(milk::math::Vector3d(0.0f, 2.0f, 0.0f));
	actor2->setScale(milk::math::Vector3d(1.0f, 1.0f, 1.0f));

	for (;;) {
		auto now = std::chrono::steady_clock::now();

		app_->update();

		if (app_->closeRequested()) {
			break;
		}

		graphicsDevice_->beginScene();

		auto dt = now - start;
		auto secs = static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(dt).count()) / 1000.0f;

		camera->setRotation(milk::math::Vector3d(0.0f, 0.03f * 3.14f * secs, 0.0f));

		actor->setRotation(milk::math::Vector3d(0.0f, 0.0f, 0.0f));
		actor->setTranslation(milk::math::Vector3d(0.0f, -1.0f, 0.0f));
		actor->setScale(milk::math::Vector3d(1.0f, 1.0f, 1.0f));

		scene.render(*graphicsDevice_);

		graphicsDevice_->endScene();
	}
}
