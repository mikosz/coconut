#include "Game.hpp"

#include <vector>
#include <cstdint>
#include <memory>
#include <chrono>

#include <coconut-tools/serialisation.hpp>

#include "coconut/milk/graphics/Shader.hpp"

#include "coconut/milk/fs.hpp"

#include "coconut/milk/system/Window.hpp"

#include "coconut/pulp/mesh/obj/Importer.hpp"

#include "coconut/pulp/renderer/Model.hpp"
#include "coconut/pulp/renderer/ModelFactory.hpp"
#include "coconut/pulp/renderer/PerspectiveLens.hpp"
#include "coconut/pulp/renderer/OrientedCamera.hpp"
#include "coconut/pulp/renderer/Scene.hpp"
#include "coconut/pulp/renderer/Actor.hpp"
#include "coconut/pulp/renderer/CommandBuffer.hpp"
#include "coconut/pulp/renderer/shader/PassFactory.hpp"

#include "coconut/pulp/world/foliage/GrassActor.hpp"

#include "globals.hpp"

using namespace coconut;
using namespace coconut::shell;
using namespace coconut::shell::game;

using namespace coconut::pulp::math_literals;

Game::Game(std::shared_ptr<milk::system::App> app) :
	app_(app),
	filesystem_(std::make_unique<milk::Filesystem>())
{
	{
		auto currentMount = std::make_unique<milk::DirectoryMount>(".", false);
		filesystem_->mount("/", std::move(currentMount), milk::Filesystem::PredecessorHidingPolicy::ADD);

		// TODO: readOnly is ignored!
		auto worldMount = std::make_unique<milk::DirectoryMount>("../coconut-pulp-world", true);
		filesystem_->mount("/", std::move(worldMount), milk::Filesystem::PredecessorHidingPolicy::ADD);
	}

	{
		milk::system::Window::Configuration configuration;
		configuration.className = "coconut::milk::graphics::device::Window";
		configuration.fullscreen = false;
		configuration.width = 800;
		configuration.height = 600;
		configuration.title = "Game window";

		window_.reset(new milk::system::Window(configuration, app_));
	}

	{
		milk::graphics::Renderer::Configuration configuration;
		configuration.debugDevice = DEBUG;
		configuration.vsync = false;
		configuration.sampleCount = std::numeric_limits<std::uint32_t>::max();
		configuration.sampleQuality = std::numeric_limits<std::uint32_t>::max();

		graphicsRenderer_.reset(new milk::graphics::Renderer(*window_, configuration));
	}
}

void Game::loop() {
	auto fs = milk::FilesystemContext(filesystem_);

	pulp::renderer::shader::PassFactory passFactory;
	// TODO: temp
#ifdef NDEBUG
	passFactory.scanCompiledShaderDirectory(fs, "Release");
#else
	passFactory.scanCompiledShaderDirectory(fs, "Debug");
#endif

	auto scene = pulp::renderer::Scene(*graphicsRenderer_);

	pulp::renderer::OrientedCameraSharedPtr camera(new pulp::renderer::OrientedCamera);

	pulp::renderer::LensSharedPtr lens(
		std::make_unique<pulp::renderer::PerspectiveLens>(
			pulp::math::Handedness::LEFT,
			1.0_rad,
			800.0f / 600.0f,
			0.001f,
			1000.0f
			)
		);

	scene.setCamera(camera);
	scene.setLens(lens);

	pulp::renderer::lighting::DirectionalLight white(
		pulp::math::Vec3(0.0f, 0.0f, 1.0f).normalised(),
		pulp::math::Vec4(0.1f, 0.1f, 0.1f, 0.0f),
		pulp::math::Vec4(1.0f, 1.0f, 1.0f, 1.0f),
		pulp::math::Vec4(1.0f, 1.0f, 1.0f, 0.0f)
		);
	scene.add(white);

	/* pulp::renderer::lighting::PointLight yellow(
		pulp::math::Vec3(0.0f, 1.5f, -3.5f),
		pulp::math::Vec3(0.0f, 1.0f, 0.0f),
		pulp::math::Vec4(0.1f, 0.0f, 0.0f, 0.0f),
		pulp::math::Vec4(0.7f, 0.0f, 0.0f, 1.0f),
		pulp::math::Vec4(0.4f, 0.0f, 0.0f, 0.0f)
		);
	scene.add(yellow); */

	pulp::renderer::ModelFactory modelFactory;
	modelFactory.scanSourceDirectory(fs, "data/models");
	modelFactory.scanModelDirectory(fs, "data/models");

	auto& inputElementFactory = passFactory.inputFactory().inputElementFactory();
	pulp::world::foliage::GrassActor::registerShaderInputElements(inputElementFactory);
	pulp::world::foliage::GrassActor::registerParameters(passFactory.shaderFactory().parameterFactory());
	pulp::world::foliage::GrassActor::registerResources(passFactory.shaderFactory().resourceFactory());

	auto grassModel = modelFactory.create("grass"/*"grass-fakeinst"*/, *graphicsRenderer_, passFactory, fs);
	
	bool once = false;
	//for (float x = -20.0f; x < 20.0f; x += 0.05f) {
	//	for (float z = -20.0f; z < 100.0f; z += 0.05f) {
	for (float x = -4.0f; x < 3.9f; x += 1.0f) {
		for (float z = -4.0f; z < 3.9f; z += 1.0f) {
			auto grassActor = std::make_shared<pulp::world::foliage::GrassActor>(pulp::Vec3{ x, 0.0, z });
			if (!once) {
				scene.add(grassActor, grassModel);
				//once = true;
			}
		}
	}

	auto& commandList = graphicsRenderer_->getImmediateCommandList(); // TODO: access to immediate context as command list
	pulp::renderer::CommandBuffer commandBuffer;

	const auto start = std::chrono::steady_clock::now();
	for (;;) {
		auto now = std::chrono::steady_clock::now();

		app_->update();

		if (app_->closeRequested()) {
			break;
		}

		graphicsRenderer_->beginScene();

		auto dt = now - start;
		auto secs = static_cast<float>(std::chrono::duration_cast<std::chrono::nanoseconds>(dt).count()) / 1000000000.0f;

		camera->reset();
		// camera->rotate(pulp::math::Vec3(0.0f, 0.09f * 3.14f * secs, 0.0f));
		camera->translate(pulp::math::Vec3(0.0f, 0.8f, 0.0f));
		//camera->rotate(pulp::math::Vec3(1.0f, 0.0f, 0.0f), 0.25_rad);
		//camera->translate(pulp::math::Vec3(0.0f, 0.0f, -5.0f));
		
		scene.render(*graphicsRenderer_, commandBuffer);

		commandBuffer.submit(commandList);
		graphicsRenderer_->submit(commandList);

		graphicsRenderer_->endScene();
	}
}
