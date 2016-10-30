#include "Game.hpp"

#include <fstream>
#include <vector>
#include <cstdint>
#include <memory>
#include <chrono>

#include <boost/filesystem.hpp>

#include <coconut-tools/serialisation/BinarySerialiser.hpp>
#include <coconut-tools/serialisation/BinaryDeserialiser.hpp>
#include <coconut-tools/serialisation/JSONDeserialiser.hpp>

#include "coconut/milk/graphics/FlexibleInputLayoutDescription.hpp"
#include "coconut/milk/graphics/Shader.hpp"

#include "coconut/pulp/model/obj/Importer.hpp"

#include "coconut/pulp/renderer/Model.hpp"
#include "coconut/pulp/renderer/PerspectiveLens.hpp"
#include "coconut/pulp/renderer/OrientedCamera.hpp"
#include "coconut/pulp/renderer/Scene.hpp"
#include "coconut/pulp/renderer/Actor.hpp"
#include "coconut/pulp/renderer/CommandBuffer.hpp"
#include "coconut/pulp/renderer/shader/PassFactory.hpp"

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

	pulp::renderer::LensSharedPtr lens(new pulp::renderer::PerspectiveLens(milk::math::Handedness::LEFT, 1.0f, 800.0f / 600.0f, 0.001f, 1000.0f));

	if (!boost::filesystem::exists("elexis.model")) {
		// std::ifstream modelIS("data/models/Daniel/craig chemise bleu/craig chemis bleu.obj");
		std::ifstream modelIS("data/models/Elexis/Blonde Elexis - nude/Blonde Elexis - nude.obj");
		// std::ifstream modelIS("data/models/cube.model");
		if (!modelIS.good()) {
			throw std::runtime_error("Failed to open model file");
		}

		// auto opener = std::make_unique<pulp::model::obj::Importer::MaterialFileOpener>("data/models/Daniel/craig chemise bleu");
		auto opener = std::make_unique<pulp::model::obj::Importer::MaterialFileOpener>("data/models/Elexis/Blonde Elexis - nude");
		// auto opener = std::make_unique<pulp::model::obj::Importer::MaterialFileOpener>("data/models/");
		pulp::model::obj::Importer importer(std::move(opener));

		auto modelData = importer.import(modelIS, "elexis");

		{
			std::ofstream modelOFS("elexis.model", std::ofstream::out | std::ofstream::binary);
			coconut_tools::serialisation::BinarySerialiser serialiser(modelOFS);
			serialiser << modelData;
		}
	}

	pulp::renderer::MaterialManager materialManager;

	std::ifstream modelIFS("elexis.model", std::ifstream::in | std::ifstream::binary);
	coconut_tools::serialisation::BinaryDeserialiser deserialiser(modelIFS);
	// std::ifstream modelIFS("cube.json", std::ifstream::in);
	// coconut_tools::serialisation::JSONDeserialiser deserialiser(modelIFS);
	pulp::model::Data modelData;
	deserialiser >> modelData;

	pulp::renderer::Scene scene(*graphicsRenderer_);
	scene.setRenderingPass(pulp::renderer::shader::PassFactory().create("sprite", *graphicsRenderer_));

	pulp::renderer::ModelSharedPtr m(new pulp::renderer::Model(modelData, *graphicsRenderer_, scene.renderingPass().inputLayoutDescription(), materialManager));

	pulp::renderer::lighting::DirectionalLight white(
		milk::math::Vector3d(-0.5f, -0.5f, 0.5f).normalised(),
		milk::math::Vector4d(0.1f, 0.1f, 0.1f, 0.0f),
		milk::math::Vector4d(0.7f, 0.7f, 0.7f, 1.0f),
		milk::math::Vector4d(0.4f, 0.4f, 0.4f, 0.0f)
		);
	scene.add(white);

	pulp::renderer::lighting::PointLight yellow(
		milk::math::Vector3d(0.0f, 1.5f, -3.5f),
		milk::math::Vector3d(0.0f, 1.0f, 0.0f),
		milk::math::Vector4d(0.1f, 0.0f, 0.0f, 0.0f),
		milk::math::Vector4d(0.7f, 0.0f, 0.0f, 1.0f),
		milk::math::Vector4d(0.4f, 0.0f, 0.0f, 0.0f)
		);
	scene.add(yellow);

	pulp::renderer::ActorSharedPtr actor(new pulp::renderer::Actor(m));

	scene.add(actor);
	scene.setCamera(camera);
	scene.setLens(lens);

	pulp::renderer::ActorSharedPtr actor2(new pulp::renderer::Actor(m));

	// scene.add(actor2);
	actor2->setRotation(milk::math::Vector3d(0.0f, 0.0f, 0.0f));
	actor2->setTranslation(milk::math::Vector3d(0.0f, 2.0f, 0.0f));
	actor2->setScale(milk::math::Vector3d(1.0f, 1.0f, 1.0f));

	pulp::model::Data floorData;
	{
		floorData.rasteriserConfiguration.cullMode = milk::graphics::CullMode::BACK;
		floorData.rasteriserConfiguration.fillMode = milk::graphics::FillMode::SOLID;
		floorData.rasteriserConfiguration.frontCounterClockwise = false;

		floorData.positions.emplace_back(2.0f, 0.0f, 2.0f);
		floorData.positions.emplace_back(2.0f, 0.0f, -2.0f);
		floorData.positions.emplace_back(-2.0f, 0.0f, 2.0f);
		floorData.positions.emplace_back(-2.0f, 0.0f, -2.0f);

		floorData.normals.emplace_back(0.0f, 1.0f, 0.0f);

		floorData.textureCoordinates.emplace_back(0.0f, 0.0f);

		floorData.phongMaterials.emplace_back();
		floorData.phongMaterials.back().ambientColour = milk::math::Vector4d(1.0f, 1.0f, 1.0f, 1.0f);
		floorData.phongMaterials.back().diffuseColour = milk::math::Vector4d(1.0f, 1.0f, 1.0f, 1.0f);
		floorData.phongMaterials.back().specularColour = milk::math::Vector4d(1.0f, 1.0f, 1.0f, 1.0f);
		floorData.phongMaterials.back().name = "floor::white";

		pulp::model::Data::DrawGroup drawGroup;

		for (size_t i = 0; i < 4; ++i) {
			drawGroup.vertices.emplace_back();
			drawGroup.vertices.back().positionIndex = i;
			drawGroup.vertices.back().normalIndex = 0;
			drawGroup.vertices.back().textureCoordinateIndex = 0;

			drawGroup.primitiveTopology = milk::graphics::PrimitiveTopology::TRIANGLE_STRIP;

			drawGroup.indices.emplace_back(i);

			drawGroup.materialId = "floor::white";
		}

		floorData.drawGroups.emplace_back(drawGroup);
	}
	auto floorModel = std::make_shared<pulp::renderer::Model>(floorData, *graphicsRenderer_, scene.renderingPass().inputLayoutDescription(), materialManager);
	auto floorActor = std::make_shared<pulp::renderer::Actor>(floorModel);
	scene.add(floorActor);

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
		// camera->rotate(milk::math::Vector3d(0.0f, 0.09f * 3.14f * secs, 0.0f));
		camera->translate(milk::math::Vector3d(0.0f, 2.0f, 0.0f));
		camera->rotate(milk::math::Vector3d(0.25f, 0.0f, 0.0f));
		camera->translate(milk::math::Vector3d(0.0f, 0.0f, -5.f));
		
		actor->setRotation(milk::math::Vector3d(0.0f, 0.09f * 3.14f * secs, 0.0f));
		// actor->setRotation(milk::math::Vector3d(0.0f, 0.0f, 0.0f));
		actor->setTranslation(milk::math::Vector3d(0.0f, -.0f, 0.0f));
		actor->setScale(milk::math::Vector3d(1.0f, 1.0f, 1.0f));

		scene.render(commandBuffer);

		commandBuffer.submit(commandList);
		graphicsRenderer_->submit(commandList);

		graphicsRenderer_->endScene();
	}
}
