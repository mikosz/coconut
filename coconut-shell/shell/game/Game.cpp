#include "Game.hpp"

#include <fstream>
#include <vector>
#include <cstdint>
#include <memory>

#include <boost/filesystem.hpp>

#include "milk/graphics/FlexibleInputLayoutDescription.hpp"
#include "milk/graphics/VertexShader.hpp"
#include "milk/graphics/PixelShader.hpp"

#include "pulp/renderer/Model.hpp"

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
	std::shared_ptr<milk::graphics::VertexShader> vertexShader;

	{
		std::ifstream ifs("sprite.v.cso", std::ios::binary);
		if (!ifs) {
			throw std::runtime_error("Failed to open the vertex shader");
		}

		auto shaderSize = boost::filesystem::file_size("sprite.v.cso");
		std::vector<char> vdata;
		vdata.resize(static_cast<size_t>(shaderSize));

		ifs.read(&vdata.front(), shaderSize);

		if (!ifs) {
			throw std::runtime_error("Failed to read the vertex shader");
		}

		milk::graphics::FlexibleInputLayoutDescription inputLayoutDesc;
		inputLayoutDesc.push(std::shared_ptr<milk::graphics::FlexibleInputLayoutDescription::PositionElement>(
			new milk::graphics::FlexibleInputLayoutDescription::PositionElement(0, milk::graphics::FlexibleInputLayoutDescription::R32G32B32_FLOAT)));

		std::shared_ptr<milk::graphics::ShaderParametersDescription> shaderParameters(
			new milk::graphics::ShaderParametersDescription(milk::graphics::ShaderParametersDescription::VERTEX_SHADER)
			);
		shaderParameters->push(
			std::shared_ptr<milk::graphics::ShaderParametersDescription::MatrixElement>(
				new milk::graphics::ShaderParametersDescription::MatrixElement(
					*graphicsDevice_,
					[]() {
						return renderable.worldTransformation();
					}
					)
				)
			);

		vertexShader.reset(
			new milk::graphics::VertexShader(
				*graphicsDevice_,
				&vdata.front(),
				vdata.size(),
				inputLayoutDesc,
				shaderParameters
				)
			);
	}

	std::shared_ptr<milk::graphics::PixelShader> pixelShader;

	{
		std::ifstream ifs("sprite.p.cso", std::ios::binary);
		if (!ifs) {
			throw std::runtime_error("Failed to open the pixel shader");
		}

		auto shaderSize = boost::filesystem::file_size("sprite.p.cso");
		std::vector<char> pdata;
		pdata.resize(static_cast<size_t>(shaderSize));

		ifs.read(&pdata.front(), shaderSize);

		if (!ifs) {
			throw std::runtime_error("Failed to read the pixel shader");
		}

		pixelShader.reset(new milk::graphics::PixelShader(*graphicsDevice_, &pdata.front(), pdata.size()));
	}

	pulp::renderer::Model m(*graphicsDevice_, vertexShader, pixelShader);

	for (;;) {
		app_->update();

		if (app_->closeRequested()) {
			break;
		}

		graphicsDevice_->beginScene();

		m.setRotation(milk::math::Vector3d(0.0f, 0.0f, 0.0f));
		m.setTranslation(milk::math::Vector3d(0.0f, 0.0f, 0.0f));
		m.setScale(milk::math::Vector3d(1.0f, 1.0f, 1.0f));

		vertexShader->update(*graphicsDevice_, m);
		m.render(*graphicsDevice_);

		graphicsDevice_->endScene();
	}
}
