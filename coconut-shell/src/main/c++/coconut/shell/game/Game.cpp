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
#include "coconut/pulp/renderer/PerspectiveLens.hpp"
#include "coconut/pulp/renderer/OrientedCamera.hpp"
#include "coconut/pulp/renderer/Scene.hpp"
#include "coconut/pulp/renderer/Actor.hpp"
#include "coconut/pulp/renderer/CommandBuffer.hpp"
#include "coconut/pulp/renderer/shader/PassFactory.hpp"

#include "coconut/pulp/world/foliage/Grass.hpp"

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

	auto fs = milk::FilesystemContext(filesystem_);

	if (!fs.exists("daniel.model")) {
	//if (true) {
		auto modelContext = fs;

		modelContext.changeWorkingDirectory("/data/models/Daniel/craig chemise bleu/");
		auto data = modelContext.load("craig chemis bleu.obj").get();
		//modelContext.changeWorkingDirectory("/data/models/");
		//auto data = modelContext.load("triangle.obj").get();

		auto modelData = pulp::mesh::obj::Importer().import(*data, modelContext);

		{
			auto modelOS = fs.overwrite("daniel.model");
			//auto modelOS = fs.overwrite("triangle.model");
			coconut_tools::serialisation::BinarySerialiser serialiser(*modelOS);
			serialiser << modelData;
		}
	}

	auto modelData = pulp::Mesh();

	{
		auto modelIS = fs.open("daniel.model");
		//auto modelIS = fs.open("triangle.model");
		coconut_tools::serialisation::BinaryDeserialiser deserialiser(*modelIS);
		deserialiser >> modelData;
	}

	pulp::renderer::shader::PassFactory passFactory;
	passFactory.scanCompiledShaderDirectory(fs, "Debug");

	auto scene = pulp::renderer::Scene(*graphicsRenderer_);

	auto m = std::make_shared<pulp::renderer::Model>(std::move(modelData), *graphicsRenderer_, passFactory, fs);

	pulp::renderer::lighting::DirectionalLight white(
		pulp::math::Vec3(-0.5f, -0.5f, 0.5f).normalised(),
		pulp::math::Vec4(0.1f, 0.1f, 0.1f, 0.0f),
		pulp::math::Vec4(0.7f, 0.7f, 0.7f, 1.0f),
		pulp::math::Vec4(0.4f, 0.4f, 0.4f, 0.0f)
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

	pulp::renderer::ActorSharedPtr actor(new pulp::renderer::Actor(m));

	scene.add(actor);
	scene.setCamera(camera);
	scene.setLens(lens);

	pulp::renderer::ActorSharedPtr actor2(new pulp::renderer::Actor(m));

	// scene.add(actor2);
	actor2->setRotation(pulp::math::Vec3(0.0f, 0.0f, 0.0f));
	actor2->setTranslation(pulp::math::Vec3(0.0f, 2.0f, 0.0f));
	actor2->setScale(pulp::math::Vec3(1.0f, 1.0f, 1.0f));

	auto grassActor = std::make_shared<pulp::world::foliage::Grass>(*graphicsRenderer_, passFactory, fs);
	scene.add(grassActor);

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
		camera->translate(pulp::math::Vec3(0.0f, 2.0f, 0.0f));
		camera->rotate(pulp::math::Vec3(1.0f, 0.0f, 0.0f), 0.25_rad);
		camera->translate(pulp::math::Vec3(0.0f, 0.0f, -5.0f));
		
		actor->setRotation(pulp::math::Vec3(0.0f, 0.09f * 3.14f * secs, 0.0f));
		// actor->setRotation(pulp::math::Vec3(0.0f, 0.0f, 0.0f));
		actor->setTranslation(pulp::math::Vec3(0.0f, -.0f, 0.0f));
		actor->setScale(pulp::math::Vec3(1.0f, 1.0f, 1.0f));

		scene.render(commandBuffer);

		commandBuffer.submit(commandList);
		graphicsRenderer_->submit(commandList);

		graphicsRenderer_->endScene();
	}
}
