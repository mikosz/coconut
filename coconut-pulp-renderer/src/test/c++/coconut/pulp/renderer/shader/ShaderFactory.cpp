#include <boost/test/auto_unit_test.hpp>

#include <memory>

#include "coconut/milk/graphics/TestSuite.hpp"

#include "coconut/milk/fs.hpp"

#include "coconut/pulp/renderer/shader/InputFactory.hpp"
#include "coconut/pulp/renderer/shader/ShaderFactory.hpp"
#include "coconut/pulp/renderer/OrientedCamera.hpp"
#include "coconut/pulp/renderer/CommandBuffer.hpp"
#include "coconut/pulp/renderer/Scene.hpp"

#include "coconut/milk/graphics/DirectXError.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;
using namespace coconut::pulp::renderer::shader;

BOOST_FIXTURE_TEST_SUITE(PulpRendererShaderShaderFactoryTestSuite, milk::graphics::TestSuite);

BOOST_AUTO_TEST_CASE(SetsConstantBuffers) {
	// init shaders
	std::shared_ptr<Pass> renderingPass;

	auto fs = std::make_shared<milk::Filesystem>();

	auto mount = std::make_unique<milk::DirectoryMount>("Debug/", true); // TODO!
	fs->mount("/", std::move(mount), milk::Filesystem::PredecessorHidingPolicy::HIDE);

	auto fsContext = milk::FilesystemContext(fs);

	{
		auto inputFactory = InputFactory();
		inputFactory.registerCompiledShader("v", "ConstantBuffers.v.cso");

		auto shaderFactory = ShaderFactory();
	
		{
			auto vertexCodeInfo = ShaderFactory::CompiledShaderInfo();
			vertexCodeInfo.compiledShaderPath = "ConstantBuffers.v.cso";
			vertexCodeInfo.shaderType = milk::graphics::ShaderType::VERTEX;

			shaderFactory.registerCompiledShader("v", vertexCodeInfo);
		}

		{
			auto pixelCodeInfo = ShaderFactory::CompiledShaderInfo();
			pixelCodeInfo.compiledShaderPath = "ConstantBuffers.p.cso";
			pixelCodeInfo.shaderType = milk::graphics::ShaderType::PIXEL;

			shaderFactory.registerCompiledShader("p", pixelCodeInfo);
		}

		auto inputLayout = inputFactory.create("v", renderer(), fsContext);
		auto vertexShader = shaderFactory.create("v", renderer(), fsContext);
		auto pixelShader = shaderFactory.create("p", renderer(), fsContext);

		renderingPass = std::make_unique<Pass>(
			std::move(inputLayout),
			std::dynamic_pointer_cast<VertexShader>(vertexShader),
			std::dynamic_pointer_cast<PixelShader>(pixelShader)
			);
	}

	// init data
	auto scene = Scene(renderer());
	auto materialManager = MaterialManager(); // TODO: wtf is this for?

	{
		// TODO: put this in a file
		auto rectangleData = pulp::model::Data();

		{
			rectangleData.rasteriserConfiguration.cullMode = milk::graphics::Rasteriser::CullMode::BACK;
			rectangleData.rasteriserConfiguration.fillMode = milk::graphics::Rasteriser::FillMode::SOLID;
			rectangleData.rasteriserConfiguration.frontCounterClockwise = false;

			rectangleData.positions.emplace_back(-1.0f, 1.0f, 0.0f);
			rectangleData.positions.emplace_back(-1.0f, -1.0f, 0.0f);
			rectangleData.positions.emplace_back(1.0f, -1.0f, 0.0f);
			rectangleData.positions.emplace_back(1.0f, 1.0f, 0.0f);

			rectangleData.normals.emplace_back(0.0f, 0.0f, -1.0f);

			rectangleData.textureCoordinates.emplace_back(0.0f, 0.0f);

			rectangleData.phongMaterials.emplace_back();
			rectangleData.phongMaterials.back().ambientColour = milk::math::Vector4d(0.01f, 0.02f, 0.03f, 0.04f);
			rectangleData.phongMaterials.back().diffuseColour = milk::math::Vector4d(0.05f, 0.06f, 0.07f, 0.08f);
			rectangleData.phongMaterials.back().specularColour = milk::math::Vector4d(0.09f, 0.1f, 0.11f, 0.12f);

			rectangleData.phongMaterials.back().name = "rectangle::white";

			auto drawGroup = pulp::model::Data::DrawGroup();

			for (size_t i = 0; i < 4; ++i) {
				drawGroup.vertices.emplace_back();
				drawGroup.vertices.back().positionIndex = i;
				drawGroup.vertices.back().normalIndex = 0;
				drawGroup.vertices.back().textureCoordinateIndex = 0;

				drawGroup.primitiveTopology = milk::graphics::PrimitiveTopology::TRIANGLE_STRIP;

				drawGroup.indices.emplace_back(i);

				drawGroup.materialId = "rectangle::white";
			}

			rectangleData.drawGroups.emplace_back(drawGroup);
		}
		
		auto model = std::make_shared<Model>(rectangleData, renderer(), renderingPass->input(), materialManager);
		auto rectangle = std::make_shared<Actor>(model);

		scene.add(rectangle);

		auto camera = std::make_unique<OrientedCamera>(); // TODO: shit api
		camera->translate(coconut::milk::math::Vector3d(0.13f, 0.14f, 0.15f));
		scene.setCamera(std::move(camera));

		{
			scene.add(lighting::DirectionalLight(
				milk::math::Vector3d(0.28f, 0.29f, 0.30f),
				milk::math::Vector4d(0.16f, 0.17f, 0.18f, 0.19f),
				milk::math::Vector4d(0.20f, 0.21f, 0.22f, 0.23f),
				milk::math::Vector4d(0.24f, 0.25f, 0.26f, 0.27f)
				));

			scene.add(lighting::DirectionalLight(
				milk::math::Vector3d(0.43f, 0.44f, 0.45f),
				milk::math::Vector4d(0.31f, 0.32f, 0.33f, 0.34f),
				milk::math::Vector4d(0.35f, 0.36f, 0.37f, 0.38f),
				milk::math::Vector4d(0.39f, 0.40f, 0.41f, 0.42f)
				));

			scene.add(lighting::PointLight(
				milk::math::Vector3d(0.46f, 0.47f, 0.48f),
				milk::math::Vector3d(0.49f, 0.50f, 0.51f),
				milk::math::Vector4d(0.52f, 0.53f, 0.54f, 0.55f),
				milk::math::Vector4d(0.56f, 0.57f, 0.58f, 0.59f),
				milk::math::Vector4d(0.60f, 0.61f, 0.62f, 0.63f)
				));
		}
	}

	scene.setRenderingPass(renderingPass);

	// render
	{
		app().update();

		renderer().beginScene();

		CommandBuffer commandBuffer;
		scene.render(commandBuffer);
		commandBuffer.submit(renderer().getImmediateCommandList());

		renderer().endScene();
	}

	BOOST_FAIL("No checks done");
}

BOOST_AUTO_TEST_SUITE_END(/* PulpRendererShaderShaderFactoryTestSuite */);
