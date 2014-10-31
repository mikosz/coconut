#include "Game.hpp"

#include "device/Window.hpp"

using namespace coconut;
using namespace coconut::shell;
using namespace coconut::shell::game;

Game::Game(std::shared_ptr<milk::graphics::device::App> app) :
	app_(app)
{
}

void Game::loop() {
	milk::graphics::device::Window::Configuration configuration;
	configuration.className = "milk::graphics::device::Window";
	configuration.fullscreen = false;
	configuration.width = 800;
	configuration.height = 600;
	configuration.title = "Game window";

	milk::graphics::device::Window window(configuration, app_);

	for (;;) {
		app_->update();

		if (app_->closeRequested()) {
			break;
		}
	}
}
