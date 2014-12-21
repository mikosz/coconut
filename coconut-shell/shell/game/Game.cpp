#include "Game.hpp"

#include <fstream>
#include <vector>
#include <cstdint>
#include <memory>
#include <chrono>

#include <boost/filesystem.hpp>

#include "milk/graphics/FlexibleInputLayoutDescription.hpp"
#include "milk/graphics/VertexShader.hpp"
#include "milk/graphics/PixelShader.hpp"

#include "pulp/renderer/Model.hpp"
#include "pulp/renderer/OrthographicLens.hpp"
#include "pulp/renderer/OrientedCamera.hpp"
#include "pulp/renderer/shader/ParameterList.hpp"
#include "pulp/renderer/shader/ParameterWriterList.hpp"
#include "pulp/renderer/shader/ObserverAwareParameterWriter.hpp"
#include "pulp/renderer/shader/ObjectAwareParameterWriter.hpp"

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
		inputLayoutDesc.push(
			std::shared_ptr<milk::graphics::FlexibleInputLayoutDescription::PositionElement>(
				new milk::graphics::FlexibleInputLayoutDescription::PositionElement(
					0,
					milk::graphics::FlexibleInputLayoutDescription::R32G32B32_FLOAT
					)
				)
			);

		vertexShader.reset(
			new milk::graphics::VertexShader(
				*graphicsDevice_,
				&vdata.front(),
				vdata.size(),
				inputLayoutDesc
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

	pulp::renderer::shader::ParameterWriterList writerList;

	pulp::renderer::OrientedCameraSharedPtr camera(new pulp::renderer::OrientedCamera);

	{
		pulp::renderer::LensSharedPtr lens(new pulp::renderer::OrthographicLens(milk::math::Handedness::LEFT, 1.0f, 1.0f, 0.1f, 100.0f));
		pulp::renderer::shader::ParameterWriterSharedPtr writer(new pulp::renderer::shader::ObserverAwareParameterWriter(camera, lens));
		writerList.add(pulp::renderer::shader::ParameterId::VIEW_MATRIX, writer);
		writerList.add(pulp::renderer::shader::ParameterId::PROJECTION_MATRIX, writer);
	}

	pulp::renderer::ModelSharedPtr m(new pulp::renderer::Model(*graphicsDevice_, vertexShader, pixelShader));

	{
		pulp::renderer::shader::ParameterWriterSharedPtr writer(new pulp::renderer::shader::ObjectAwareParameterWriter(m));
		writerList.add(pulp::renderer::shader::ParameterId::WORLD_MATRIX, writer);
	}

	pulp::renderer::shader::ParameterList parameterList;

	{
		pulp::renderer::shader::ParameterList::BufferDescription desc;
		desc.parameterIds.push_back(pulp::renderer::shader::ParameterId::WORLD_MATRIX);
		parameterList.add(*graphicsDevice_, desc);
	}

	{
		pulp::renderer::shader::ParameterList::BufferDescription desc;
		desc.parameterIds.push_back(pulp::renderer::shader::ParameterId::VIEW_MATRIX);
		parameterList.add(*graphicsDevice_, desc);
	}

	{
		pulp::renderer::shader::ParameterList::BufferDescription desc;
		desc.parameterIds.push_back(pulp::renderer::shader::ParameterId::PROJECTION_MATRIX);
		parameterList.add(*graphicsDevice_, desc);
	}

	std::chrono::monotonic_clock::time_point start = std::chrono::monotonic_clock::now();

	camera->setTranslation(milk::math::Vector3d(0.0f, 0.0f, 10.0f));

	for (;;) {
		std::chrono::monotonic_clock::time_point now = std::chrono::monotonic_clock::now();

		app_->update();

		if (app_->closeRequested()) {
			break;
		}

		graphicsDevice_->beginScene();

		std::chrono::monotonic_clock::duration dt = now - start;
		float secs = static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(dt).count()) / 1000.0f;

		camera->setRotation(milk::math::Vector3d(0.0f, 3.14f * secs, 0.0f));

		m->setRotation(milk::math::Vector3d(0.0f, 0.0f, 3.14f * secs));
		m->setTranslation(milk::math::Vector3d(0.0f, 0.0f, 0.0f));
		m->setScale(milk::math::Vector3d(1.0f, 1.0f, 1.0f));

		parameterList.bind(*graphicsDevice_, writerList, milk::graphics::Buffer::ShaderType::VERTEX);

		m->render(*graphicsDevice_);

		graphicsDevice_->endScene();
	}
}
