#include "ShaderFactory.hpp"

#include <fstream>
#include <stdexcept>
#include <vector>

#include <boost/filesystem.hpp>

#include "coconut/milk/graphics/VertexShader.hpp"
#include "coconut/milk/graphics/PixelShader.hpp"
#include "coconut/milk/graphics/FlexibleInputLayoutDescription.hpp"

#include "coconut/milk/math/Matrix.hpp"

#include "CallbackUpdateableParameter.hpp"
#include "Pass.hpp"
#include "Resource.hpp"
#include "../Actor.hpp"
#include "../Scene.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;
using namespace coconut::pulp::renderer::shader;

ShaderFactory::ShaderFactory() {
}

PassUniquePtr ShaderFactory::createShader(milk::graphics::Device& graphicsDevice, ShaderId) {
	milk::graphics::InputLayoutUniquePtr inputLayout;
	ShaderUniquePtr vertexShader;
	ShaderUniquePtr pixelShader;

	{
		milk::graphics::ShaderType shaderType;
		Shader::SceneParameters sceneParameters;
		Shader::ActorParameters actorParameters;
		Shader::MaterialParameters materialParameters;
		Shader::Resources resources;
		milk::graphics::VertexShaderSharedPtr binaryShader;

		shaderType = milk::graphics::ShaderType::VERTEX;

		std::ifstream ifs("Debug/sprite.v.cso", std::ios::binary);
		if (!ifs) {
			throw std::runtime_error("Failed to open the vertex shader");
		}

		auto shaderSize = boost::filesystem::file_size("Debug/sprite.v.cso");
		std::vector<char> vdata;
		vdata.resize(static_cast<size_t>(shaderSize));

		ifs.read(&vdata.front(), shaderSize);

		if (!ifs) {
			throw std::runtime_error("Failed to read the vertex shader");
		}

		auto inputLayoutDesc = std::make_unique<milk::graphics::FlexibleInputLayoutDescription>();
		inputLayoutDesc->push(
			std::make_shared<milk::graphics::FlexibleInputLayoutDescription::PositionElement>(
				0, milk::graphics::FlexibleInputLayoutDescription::Format::R32G32B32A32_FLOAT
				)
			);
		inputLayoutDesc->push(
			std::make_shared<milk::graphics::FlexibleInputLayoutDescription::TextureCoordinatesElement>(
				0 , milk::graphics::FlexibleInputLayoutDescription::Format::R32G32_FLOAT
				)
			);

		inputLayout = std::make_unique<milk::graphics::InputLayout>(std::move(inputLayoutDesc), graphicsDevice, &vdata.front(), vdata.size());

		binaryShader.reset(
			new milk::graphics::VertexShader(
				graphicsDevice,
				&vdata.front(),
				vdata.size()
				)
			);

		{
			ActorParameterSharedPtr actorParameter(
				new CallbackUpdateableParameter<Actor, milk::math::Matrix>(
					graphicsDevice,
					[](const Actor& actor, milk::math::Matrix& result) {
						result = actor.worldTransformation().transposed();
					}
					)
				);
			actorParameters.insert(std::make_pair(0, actorParameter));
		}

		{
			SceneParameterSharedPtr viewParameter(
				new CallbackUpdateableParameter<Scene, milk::math::Matrix>(
					graphicsDevice,
					[](const Scene& scene, milk::math::Matrix& result) {
						result = scene.camera().viewTransformation().transposed();
					}
					)
				);
			sceneParameters.insert(std::make_pair(1, viewParameter));
		}

		{
			SceneParameterSharedPtr projectionParameter(
				new CallbackUpdateableParameter<Scene, milk::math::Matrix>(
					graphicsDevice,
					[](const Scene& scene, milk::math::Matrix& result) {
						result = scene.lens().projectionTransformation().transposed();
					}
					)
				);
			sceneParameters.insert(std::make_pair(2, projectionParameter));
		}

		vertexShader = std::make_unique<Shader>(
			binaryShader,
			shaderType,
			std::move(sceneParameters),
			std::move(actorParameters),
			std::move(materialParameters),
			std::move(resources)
			);
	}
	
	{
		milk::graphics::ShaderType shaderType;
		Shader::SceneParameters sceneParameters;
		Shader::ActorParameters actorParameters;
		Shader::MaterialParameters materialParameters;
		Shader::Resources resources;
		milk::graphics::ShaderSharedPtr binaryShader;

		shaderType = milk::graphics::ShaderType::PIXEL;

		std::ifstream ifs("Debug/sprite.p.cso", std::ios::binary);
		if (!ifs) {
			throw std::runtime_error("Failed to open the pixel shader");
		}

		auto shaderSize = boost::filesystem::file_size("Debug/sprite.p.cso");
		std::vector<char> pdata;
		pdata.resize(static_cast<size_t>(shaderSize));

		ifs.read(&pdata.front(), shaderSize);

		if (!ifs) {
			throw std::runtime_error("Failed to read the pixel shader");
		}

		binaryShader.reset(new milk::graphics::PixelShader(graphicsDevice, &pdata.front(), pdata.size()));

		{
			MaterialParameterSharedPtr materialParameter(
				new CallbackUpdateableParameter<material::Material, milk::math::Vector4d::ShaderParameter>(
					graphicsDevice,
					[](const material::Material& material, milk::math::Vector4d::ShaderParameter& result) {
						result = material.diffuseColour().shaderParameter();
					}
					)
				);
			materialParameters.insert(std::make_pair(0, materialParameter));
		}

		{
			auto resource = std::make_shared<Resource>(
				[](milk::graphics::Device& graphicsDevice, const RenderingContext& context) {
					return context.material->diffuseMap().asShaderResource(graphicsDevice);
				}
				);
			resources.insert(std::make_pair(0, resource));
		}

		pixelShader = std::make_unique<Shader>(
			binaryShader,
			shaderType,
			std::move(sceneParameters),
			std::move(actorParameters),
			std::move(materialParameters),
			std::move(resources)
			);
	}

	return std::make_unique<Pass>(std::move(inputLayout), std::move(vertexShader), std::move(pixelShader));
}
