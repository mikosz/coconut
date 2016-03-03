#include "ShaderFactory.hpp"

#include <fstream>
#include <stdexcept>
#include <vector>

#include <boost/filesystem.hpp>

#include <coconut-tools/configuration/hierarchical/HierarchicalConfiguration.hpp>
#include "coconut-tools/configuration/readers/HierarchicalConfigurationReader.hpp"
#include <coconut-tools/configuration/parsers/JSONParser.hpp>

#include "coconut/milk/graphics/VertexShader.hpp"
#include "coconut/milk/graphics/PixelShader.hpp"
#include "coconut/milk/graphics/FlexibleInputLayoutDescription.hpp"

#include "coconut/milk/math/Matrix.hpp"

#include "ArrayParameter.hpp"
#include "CallbackParameter.hpp"
#include "StructuredParameter.hpp"
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

PassUniquePtr ShaderFactory::createShaderPass(milk::graphics::Device& graphicsDevice, PassId passId) {
	milk::graphics::InputLayoutUniquePtr inputLayout;
	ShaderUniquePtr vertexShader;
	ShaderUniquePtr pixelShader;

	createShader(graphicsDevice, "forward-opaque.vertex");

	{
		milk::graphics::ShaderType shaderType;
		Shader::SceneData sceneData;
		Shader::ActorData actorData;
		Shader::MaterialData materialData;
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
				0, milk::graphics::FlexibleInputLayoutDescription::Format::R32G32_FLOAT
				)
			);
		inputLayoutDesc->push(
			std::make_shared<milk::graphics::FlexibleInputLayoutDescription::NormalElement>(
				0, milk::graphics::FlexibleInputLayoutDescription::Format::R32G32B32_FLOAT
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
			auto worldParameter =
				std::make_unique<CallbackParameter<milk::math::Matrix, Actor>>(
					[](milk::math::Matrix& result, const Actor& actor) {
						result = actor.worldTransformation().transposed();
					}
					)
				;

			auto worldInvTransParameter =
				std::make_unique<CallbackParameter<milk::math::Matrix, Actor>>(
					[](milk::math::Matrix& result, const Actor& actor) {
						result = actor.worldTransformation().inverted();
					}
					)
				;

			StructuredParameter<Actor>::Subparameters actorFields;
			actorFields.reserve(2);
			actorFields.emplace_back(std::move(worldParameter));
			actorFields.emplace_back(std::move(worldInvTransParameter));

			auto actorParameter = std::make_unique<StructuredParameter<Actor>>(std::move(actorFields));

			actorData.emplace_back(std::make_unique<ConstantBuffer<Actor>>(graphicsDevice, shaderType, 0, std::move(actorParameter)));
		}

		{
			auto viewParameter =
				std::make_unique<CallbackParameter<milk::math::Matrix, Scene>>(
					[](milk::math::Matrix& result, const Scene& scene) {
						result = scene.camera().viewTransformation().transposed();
					}
					)
				;
			sceneData.emplace_back(std::make_unique<ConstantBuffer<Scene>>(graphicsDevice, shaderType, 1, std::move(viewParameter)));
		}

		{
			auto projectionParameter =
				std::make_unique<CallbackParameter<milk::math::Matrix, Scene>>(
					[](milk::math::Matrix& result, const Scene& scene) {
						result = scene.lens().projectionTransformation().transposed();
					}
					)
				;
			sceneData.emplace_back(std::make_unique<ConstantBuffer<Scene>>(graphicsDevice, shaderType, 2, std::move(projectionParameter)));
		}

		vertexShader = std::make_unique<Shader>(
			binaryShader,
			shaderType,
			std::move(sceneData),
			std::move(actorData),
			std::move(materialData),
			std::move(resources)
			);
	}
	
	{
		milk::graphics::ShaderType shaderType;
		Shader::SceneData sceneData;
		Shader::ActorData actorData;
		Shader::MaterialData groupData; // TODO: change type to GroupParameters and dont restrict param to material (?)
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
			auto eyeParameter =
				std::make_unique<CallbackParameter<milk::math::Vector3d::ShaderParameter, Scene>>(
					[](milk::math::Vector3d::ShaderParameter& result, const Scene& scene) {
						result = scene.camera().position().shaderParameter();
					}
					)
				;

			auto directionalLightCountParameter =
				std::make_unique<CallbackParameter<std::uint32_t, Scene>>(
					[](std::uint32_t& result, const Scene& scene) {
						result = static_cast<std::uint32_t>(scene.directionalLights().size()); // TODO: limit number of lights
					}
					)
				;
			
			auto ambientParameter =
				std::make_unique<CallbackParameter<milk::math::Vector4d::ShaderParameter, Scene, size_t>>(
					[](milk::math::Vector4d::ShaderParameter& result, const Scene& scene, size_t lightIndex) {
						if (scene.directionalLights().size() > lightIndex) {
							result = scene.directionalLights()[lightIndex].ambientColour().shaderParameter();
						}
					}
					)
				;
			auto diffuseParameter =
				std::make_unique<CallbackParameter<milk::math::Vector4d::ShaderParameter, Scene, size_t>>(
					[](milk::math::Vector4d::ShaderParameter& result, const Scene& scene, size_t lightIndex) {
						if (scene.directionalLights().size() > lightIndex) {
							result = scene.directionalLights()[lightIndex].diffuseColour().shaderParameter();
						}
					}
					)
				;
			auto specularParameter =
				std::make_unique<CallbackParameter<milk::math::Vector4d::ShaderParameter, Scene, size_t>>(
					[](milk::math::Vector4d::ShaderParameter& result, const Scene& scene, size_t lightIndex) {
						if (scene.directionalLights().size() > lightIndex) {
							result = scene.directionalLights()[lightIndex].specularColour().shaderParameter();
						}
					}
					)
				;
			
			auto directionParameter =
				std::make_unique<CallbackParameter<milk::math::Vector3d::ShaderParameter, Scene, size_t>>(
					[](milk::math::Vector3d::ShaderParameter& result, const Scene& scene, size_t lightIndex) {
						if (scene.directionalLights().size() > lightIndex) {
							result = scene.directionalLights()[lightIndex].direction().shaderParameter();
						}
					}
					)
				;

			auto paddingParameter =
				std::make_unique<CallbackParameter<float, Scene, size_t>>(
					[](float&, const Scene&, size_t) {
					}
					)
				;

			StructuredParameter<Scene, size_t>::Subparameters directionalLightFields;
			directionalLightFields.reserve(5);
			directionalLightFields.emplace_back(std::move(ambientParameter));
			directionalLightFields.emplace_back(std::move(diffuseParameter));
			directionalLightFields.emplace_back(std::move(specularParameter));
			directionalLightFields.emplace_back(std::move(directionParameter));
			directionalLightFields.emplace_back(std::move(paddingParameter));

			auto directionalLightParameter =
				std::make_unique<StructuredParameter<Scene, size_t>>(
					std::move(directionalLightFields)// TODO: could structured parameter handle padding?
					)
				;

			auto directionalLightsParameterArray =
				std::make_unique<ArrayParameter<Scene>>(
					std::move(directionalLightParameter), // TODO: padding!
					3
					)
				;

			StructuredParameter<Scene>::Subparameters lightFields;
			lightFields.reserve(3);
			lightFields.emplace_back(std::move(eyeParameter));
			lightFields.emplace_back(std::move(directionalLightCountParameter));
			lightFields.emplace_back(std::move(directionalLightsParameterArray));

			auto lightsParameter = 
				std::make_unique<StructuredParameter<Scene>>(
					lightFields
				);

			sceneData.emplace_back(std::make_unique<ConstantBuffer<Scene>>(graphicsDevice, shaderType, 0, std::move(lightsParameter)));
		}

		{
			auto ambientParameter =
				std::make_unique<CallbackParameter<milk::math::Vector4d::ShaderParameter, material::Material>>(
					[](milk::math::Vector4d::ShaderParameter& result, const material::Material& material) {
						result = material.ambientColour().shaderParameter();
					}
					)
				;
			auto diffuseParameter =
				std::make_unique<CallbackParameter<milk::math::Vector4d::ShaderParameter, material::Material>>(
					[](milk::math::Vector4d::ShaderParameter& result, const material::Material& material) {
						result = material.diffuseColour().shaderParameter();
					}
					)
				;
			auto specularParameter =
				std::make_unique<CallbackParameter<milk::math::Vector4d::ShaderParameter, material::Material>>(
					[](milk::math::Vector4d::ShaderParameter& result, const material::Material& material) {
						result = material.specularColour().shaderParameter();
					}
					)
				;

			StructuredParameter<material::Material>::Subparameters materialFields;
			materialFields.reserve(3);
			materialFields.emplace_back(std::move(ambientParameter));
			materialFields.emplace_back(std::move(diffuseParameter));
			materialFields.emplace_back(std::move(specularParameter));

			auto materialParameter = std::make_unique<StructuredParameter<material::Material>>(
				std::move(materialFields)
				);

			groupData.emplace_back(std::make_unique<ConstantBuffer<material::Material>>(graphicsDevice, shaderType, 2, std::move(materialParameter)));
			// TODO: verify that two constant buffers don't share a slot
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
			std::move(sceneData),
			std::move(actorData),
			std::move(groupData),
			std::move(resources)
			);
	}

	return std::make_unique<Pass>(std::move(inputLayout), std::move(vertexShader), std::move(pixelShader));
}

ShaderUniquePtr ShaderFactory::createShader(milk::graphics::Device& graphicsDevice, ShaderId shaderId) {
	coconut_tools::configuration::parsers::JSONParser parser;
	coconut_tools::configuration::readers::HierarchicalConfigurationReader reader;

	auto configuration = coconut_tools::configuration::hierarchical::HierarchicalConfiguration::create();

	reader.read(parser, boost::filesystem::path("data/shaders") / (shaderId + ".cfg.json"), configuration.get()); // TODO: get path from config, filename?

	const auto shaderType = configuration->getAs<std::string>("type");

	if (shaderType == "vertex") {
		std::make_unique<Shader>(
			binaryShader,
			shaderType
			);
	} else {
		assert(false);
	}

	return nullptr;
}
