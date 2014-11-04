#include "Game.hpp"

#include <fstream>
#include <vector>
#include <cstdint>
#include <filesystem>
#include <memory>

#include "milk/graphics/FlexibleInputLayoutDescription.hpp"
#include "milk/graphics/VertexShader.hpp"
#include "milk/graphics/PixelShader.hpp"

#include "pulp/model/Model.hpp"

#include "globals.hpp"
#include "milk/system/Window.hpp"

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
	std::ifstream ifs("sprite.v.cso", std::ios::binary);
	if (!ifs) {
		throw std::runtime_error("Failed to read the vertex shader");
	}
	std::vector<char> vdata;
	vdata.push_back(' ');
	ifs.read(&vdata.back(), 1);

	milk::graphics::FlexibleInputLayoutDescription inputLayoutDesc;
	inputLayoutDesc.push(std::shared_ptr<milk::graphics::FlexibleInputLayoutDescription::PositionElement>(
		new milk::graphics::FlexibleInputLayoutDescription::PositionElement(0, milk::graphics::FlexibleInputLayoutDescription::R32G32B32_FLOAT)));

	std::shared_ptr<milk::graphics::VertexShader> vertexShader(new milk::graphics::VertexShader(*graphicsDevice_, &vdata.front(), vdata.size(), inputLayoutDesc));

	/* std::vector<std::uint16_t> idata;
	idata.push_back(0);
	idata.push_back(1);
	idata.push_back(2);

	std::shared_ptr<milk::graphics::PixelShader> pixelShader(new milk::graphics::PixelShader(*graphicsDevice_, &idata.front(), idata.size()));

	pulp::model::Model m(*graphicsDevice_, vertexShader, pixelShader); */

	for (;;) {
		app_->update();

		if (app_->closeRequested()) {
			break;
		}

		graphicsDevice_->beginScene();
		// m.render(*graphicsDevice_);
		graphicsDevice_->endScene();
	}
}
