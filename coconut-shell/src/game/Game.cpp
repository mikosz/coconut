#include "Game.hpp"

#include "globals.hpp"
#include "system/Window.hpp"

using namespace coconut;
using namespace coconut::shell;
using namespace coconut::shell::game;

Game::Game(std::shared_ptr<milk::system::App> app) :
	app_(app)
{
	{
		milk::system::Window::Configuration configuration;
		configuration.className = "milk::graphics::device::Window";
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
	for (;;) {
		app_->update();

		if (app_->closeRequested()) {
			break;
		}

		graphicsDevice_->beginScene();
		graphicsDevice_->endScene();
	}
}
