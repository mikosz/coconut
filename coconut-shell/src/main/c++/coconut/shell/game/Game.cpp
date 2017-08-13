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
#include "coconut/pulp/world/World.hpp"

#include "globals.hpp"

using namespace coconut;
using namespace coconut::shell;
using namespace coconut::shell::game;

using namespace coconut::pulp::math_literals;

Game::Game(
	std::shared_ptr<milk::system::App> app,
	const coconut_tools::configuration::hierarchical::HierarchicalConfigurationSharedPtr& configuration
	) :
	app_(app),
	filesystem_(std::make_unique<milk::Filesystem>())
{
	auto mountConfigs = coconut_tools::configuration::hierarchical::HierarchicalConfiguration::Nodes();
	configuration->getAll("mounts/mount", &mountConfigs);
	for (const auto& mountConfig : mountConfigs) {
		const auto mountPoint = mountConfig->getAs<std::string>("mount-point");
		const auto filesystemPath = mountConfig->getAs<std::string>("filesystem-path");
		const auto readOnly = mountConfig->getAs<bool>("read-only");
		const auto hidingPolicyStr = mountConfig->getAs<std::string>("hiding-policy");
		
		auto hidingPolicy = milk::Filesystem::PredecessorHidingPolicy();
		fromString(hidingPolicy, hidingPolicyStr);

		// TODO: readOnly is ignored!
		// TODO: hidingPolicy could be passed to mount's constructor?
		auto mount = std::make_unique<milk::DirectoryMount>(filesystemPath, readOnly);
		filesystem_->mount(mountPoint, std::move(mount), hidingPolicy);
	}

	{
		auto windowConf = milk::system::Window::Configuration();
		windowConf.className = "coconut::milk::graphics::device::Window";
		windowConf.fullscreen = false;
		windowConf.width = 800;
		windowConf.height = 600;
		windowConf.title = "Game window";

		window_.reset(new milk::system::Window(windowConf, app_));
	}

	{
		auto rendererConf = milk::graphics::Renderer::Configuration();
		rendererConf.debugDevice = DEBUG;
		rendererConf.vsync = false;
		rendererConf.sampleCount = std::numeric_limits<std::uint32_t>::max();
		rendererConf.sampleQuality = std::numeric_limits<std::uint32_t>::max();

		graphicsRenderer_.reset(new milk::graphics::Renderer(*window_, rendererConf));
	}
}

void Game::loop() {
	auto fs = milk::FilesystemContext(filesystem_);

	pulp::renderer::shader::PassFactory passFactory;
	passFactory.scanShaderCodeDirectory(fs, "shaders");
	passFactory.scanShaderCodeDirectory(fs, "shaders/foliage"); // TODO: why is it not recursive?

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
		pulp::math::Vec3(-1.0f, -1.0f, 1.0f).normalised(),
		pulp::math::Vec4(0.1f, 0.1f, 0.1f, 0.0f),
		pulp::math::Vec4(1.0f, 1.0f, 1.0f, 1.0f),
		pulp::math::Vec4(1.0f, 1.0f, 1.0f, 0.0f)
		);
	scene.add(white);

	/*pulp::renderer::lighting::DirectionalLight red(
		pulp::math::Vec3(0.0f, 0.0f, 1.0f),
		pulp::math::Vec4(0.1f, 0.1f, 0.1f, 0.0f),
		pulp::math::Vec4(1.0f, 0.0f, 0.0f, 1.0f),
		pulp::math::Vec4(1.0f, 0.0f, 0.0f, 0.0f)
	);
	scene.add(red);*/

	/* pulp::renderer::lighting::PointLight yellow(
		pulp::math::Vec3(0.0f, 1.5f, -3.5f),
		pulp::math::Vec3(0.0f, 1.0f, 0.0f),
		pulp::math::Vec4(0.1f, 0.0f, 0.0f, 0.0f),
		pulp::math::Vec4(0.7f, 0.0f, 0.0f, 1.0f),
		pulp::math::Vec4(0.4f, 0.0f, 0.0f, 0.0f)
		);
	scene.add(yellow); */

	pulp::renderer::ModelFactory modelFactory;
	modelFactory.scanSourceDirectory(fs, "models");
	modelFactory.scanModelDirectory(fs, "models");

	auto world = pulp::world::World(*graphicsRenderer_, scene, passFactory, modelFactory, fs);

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

		const auto camX = 0.0f;
		const auto camY = 1.0f; // 4.0f + 0.25f * secs;
		const auto camZ = 0.5f * secs;
		//const auto camZ = 0.0f;

		const auto terrainHeight = world.terrain().heightmap().heightAt(camX, camZ);

		camera->reset();
		camera->translate(pulp::math::Vec3(camX, camY + terrainHeight, camZ));
		camera->rotate(pulp::math::Vec3(0.0f, 1.0f, 0.0f), pulp::math::radians(0.03f * secs));
		
		auto passContext = pulp::renderer::PassContext();
		passContext.graphicsRenderer = graphicsRenderer_.get();
		world.bindShaderProperties(passContext.properties);

		passContext.properties.bind("globalTime", secs);

		scene.render(std::move(passContext), commandBuffer);

		commandBuffer.submit(commandList);
		graphicsRenderer_->submit(commandList);

		graphicsRenderer_->endScene();
	}
}
