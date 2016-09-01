#include "ShaderFactory.hpp"

#include <fstream>
#include <stdexcept>
#include <vector>

#include <boost/filesystem.hpp>

#include "coconut/milk/graphics/Shader.hpp"
#include "coconut/milk/graphics/FlexibleInputLayoutDescription.hpp"

#include "coconut/milk/math/Matrix.hpp"

#include "ArrayParameter.hpp"
#include "CallbackParameter.hpp"
#include "StructuredParameter.hpp"
#include "Pass.hpp"
#include "Resource.hpp"
#include "../Actor.hpp"
#include "../Scene.hpp"
#include "../PhongMaterial.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;
using namespace coconut::pulp::renderer::shader;

ShaderFactory::ShaderFactory() {
}

PassUniquePtr ShaderFactory::createShader(milk::graphics::Renderer& graphicsRenderer, ShaderId) {
	milk::graphics::InputLayoutUniquePtr inputLayout;
	VertexShaderUniquePtr vertexShader;
	PixelShaderUniquePtr pixelShader;

	{
		milk::graphics::ShaderType shaderType;
		VertexShader::SceneData sceneData;
		VertexShader::ActorData actorData;
		VertexShader::MaterialData materialData;
		VertexShader::Resources resources;
		milk::graphics::VertexShaderUniquePtr binaryShader;

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

		inputLayout = std::make_unique<milk::graphics::InputLayout>(std::move(inputLayoutDesc), graphicsRenderer, &vdata.front(), vdata.size());

		binaryShader = std::make_unique<milk::graphics::VertexShader>(
				graphicsRenderer,
				&vdata.front(),
				vdata.size()
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

			auto actorParameter = std::make_unique<StructuredParameter<Actor>>(std::move(layoutSubparameters<Actor>(actorFields)));

			actorData.emplace_back(std::make_unique<ConstantBuffer<Actor>>(graphicsRenderer, shaderType, 0, std::move(actorParameter)));
		}

		{
			auto viewParameter =
				std::make_unique<CallbackParameter<milk::math::Matrix, Scene>>(
					[](milk::math::Matrix& result, const Scene& scene) {
						result = scene.camera().viewTransformation().transposed();
					}
					)
				;
			sceneData.emplace_back(std::make_unique<ConstantBuffer<Scene>>(graphicsRenderer, shaderType, 1, std::move(viewParameter)));
		}

		{
			auto projectionParameter =
				std::make_unique<CallbackParameter<milk::math::Matrix, Scene>>(
					[](milk::math::Matrix& result, const Scene& scene) {
						result = scene.lens().projectionTransformation().transposed();
					}
					)
				;
			sceneData.emplace_back(std::make_unique<ConstantBuffer<Scene>>(graphicsRenderer, shaderType, 2, std::move(projectionParameter)));
		}

		vertexShader = std::make_unique<VertexShader>(
			*binaryShader,
			std::move(sceneData),
			std::move(actorData),
			std::move(materialData),
			std::move(resources)
			);
	}
	
	{
		milk::graphics::ShaderType shaderType;
		PixelShader::SceneData sceneData;
		PixelShader::ActorData actorData;
		PixelShader::MaterialData groupData; // TODO: change type to GroupParameters and dont restrict param to material (?)
		PixelShader::Resources resources;
		milk::graphics::PixelShaderUniquePtr binaryShader;

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

		binaryShader = std::make_unique<milk::graphics::PixelShader>(
			graphicsRenderer,
			&pdata.front(),
			pdata.size()
			);

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

			StructuredParameter<Scene, size_t>::Subparameters directionalLightFields;
			directionalLightFields.reserve(5);
			directionalLightFields.emplace_back(std::move(ambientParameter));
			directionalLightFields.emplace_back(std::move(diffuseParameter));
			directionalLightFields.emplace_back(std::move(specularParameter));
			directionalLightFields.emplace_back(std::move(directionParameter));

			auto directionalLightParameter =
				std::make_unique<StructuredParameter<Scene, size_t>>(
					std::move(layoutSubparameters<Scene, size_t>(directionalLightFields))
					)
				;

			auto directionalLightsParameterArray =
				std::make_unique<ArrayParameter<Scene>>(
					std::move(directionalLightParameter),
					3
					)
				;

			auto pointLightCountParameter =
				std::make_unique<CallbackParameter<std::uint32_t, Scene>>(
					[](std::uint32_t& result, const Scene& scene) {
						result = static_cast<std::uint32_t>(scene.pointLights().size()); // TODO: limit number of lights
					}
					)
				;

			auto pPositionParameter =
				std::make_unique<CallbackParameter<milk::math::Vector3d::ShaderParameter, Scene, size_t>>(
					[](milk::math::Vector3d::ShaderParameter& result, const Scene& scene, size_t lightIndex) {
						if (scene.pointLights().size() > lightIndex) {
							result = scene.pointLights()[lightIndex].position().shaderParameter();
						}
					}
					)
				;
			auto pAttenuationParameter =
				std::make_unique<CallbackParameter<milk::math::Vector3d::ShaderParameter, Scene, size_t>>(
					[](milk::math::Vector3d::ShaderParameter& result, const Scene& scene, size_t lightIndex) {
						if (scene.pointLights().size() > lightIndex) {
							result = scene.pointLights()[lightIndex].attenuation().shaderParameter();
						}
					}
					)
				;
			auto pAmbientParameter =
				std::make_unique<CallbackParameter<milk::math::Vector4d::ShaderParameter, Scene, size_t>>(
					[](milk::math::Vector4d::ShaderParameter& result, const Scene& scene, size_t lightIndex) {
						if (scene.pointLights().size() > lightIndex) {
							result = scene.pointLights()[lightIndex].ambientColour().shaderParameter();
						}
					}
					)
				;
			auto pDiffuseParameter =
				std::make_unique<CallbackParameter<milk::math::Vector4d::ShaderParameter, Scene, size_t>>(
					[](milk::math::Vector4d::ShaderParameter& result, const Scene& scene, size_t lightIndex) {
						if (scene.pointLights().size() > lightIndex) {
							result = scene.pointLights()[lightIndex].diffuseColour().shaderParameter();
						}
					}
					)
				;
			auto pSpecularParameter =
				std::make_unique<CallbackParameter<milk::math::Vector4d::ShaderParameter, Scene, size_t>>(
					[](milk::math::Vector4d::ShaderParameter& result, const Scene& scene, size_t lightIndex) {
						if (scene.pointLights().size() > lightIndex) {
							result = scene.pointLights()[lightIndex].specularColour().shaderParameter();
						}
					}
					)
				;

			StructuredParameter<Scene, size_t>::Subparameters pointLightFields;
			pointLightFields.reserve(6);
			pointLightFields.emplace_back(std::move(pPositionParameter));
			pointLightFields.emplace_back(std::move(pAttenuationParameter));
			pointLightFields.emplace_back(std::move(pAmbientParameter));
			pointLightFields.emplace_back(std::move(pDiffuseParameter));
			pointLightFields.emplace_back(std::move(pSpecularParameter));

			auto pointLightParameter =
				std::make_unique<StructuredParameter<Scene, size_t>>(
					std::move(layoutSubparameters<Scene, size_t>(std::move(pointLightFields)))
					)
				;
			auto pointLightsParameterArray =
				std::make_unique<ArrayParameter<Scene>>(
					std::move(pointLightParameter),
					3
					)
				;

			StructuredParameter<Scene>::Subparameters lightFields;
			lightFields.reserve(6);
			lightFields.emplace_back(std::move(eyeParameter));
			lightFields.emplace_back(std::move(directionalLightCountParameter));
			lightFields.emplace_back(std::move(directionalLightsParameterArray));
			lightFields.emplace_back(std::move(pointLightCountParameter));
			lightFields.emplace_back(std::move(pointLightsParameterArray));

			auto lightsParameter = 
				std::make_unique<StructuredParameter<Scene>>(
					std::move(layoutSubparameters<Scene>(lightFields))
				);

			sceneData.emplace_back(std::make_unique<ConstantBuffer<Scene>>(graphicsRenderer, shaderType, 0, std::move(lightsParameter)));
		}

		{
			auto ambientParameter =
				std::make_unique<CallbackParameter<milk::math::Vector4d::ShaderParameter, Material>>(
					[](milk::math::Vector4d::ShaderParameter& result, const Material& material) {
						assert(material.type() == PhongMaterial::TYPE);
						const auto& phongMaterial = reinterpret_cast<const PhongMaterial&>(material);
						result = phongMaterial.ambientColour().shaderParameter();
					}
					)
				;
			auto diffuseParameter =
				std::make_unique<CallbackParameter<milk::math::Vector4d::ShaderParameter, Material>>(
					[](milk::math::Vector4d::ShaderParameter& result, const Material& material) {
						assert(material.type() == PhongMaterial::TYPE);
						const auto& phongMaterial = reinterpret_cast<const PhongMaterial&>(material);
						result = phongMaterial.diffuseColour().shaderParameter();
					}
					)
				;
			auto specularParameter =
				std::make_unique<CallbackParameter<milk::math::Vector4d::ShaderParameter, Material>>(
					[](milk::math::Vector4d::ShaderParameter& result, const Material& material) {
						assert(material.type() == PhongMaterial::TYPE);
						const auto& phongMaterial = reinterpret_cast<const PhongMaterial&>(material);
						result = phongMaterial.specularColour().shaderParameter();
					}
					)
				;

			StructuredParameter<Material>::Subparameters materialFields;
			materialFields.reserve(3);
			materialFields.emplace_back(std::move(ambientParameter));
			materialFields.emplace_back(std::move(diffuseParameter));
			materialFields.emplace_back(std::move(specularParameter));

			auto materialParameter = std::make_unique<StructuredParameter<Material>>(
				std::move(layoutSubparameters<Material>(materialFields))
				);

			groupData.emplace_back(std::make_unique<ConstantBuffer<Material>>(graphicsRenderer, shaderType, 2, std::move(materialParameter)));
			// TODO: verify that two constant buffers don't share a slot
		}

		{
			auto resource = std::make_shared<Resource>(
				[](const PassContext& context) {
					assert(context.material->type() == PhongMaterial::TYPE);
					const auto& phongMaterial = reinterpret_cast<const PhongMaterial&>(*context.material);
					if (phongMaterial.hasDiffuseMap()) {
						return &phongMaterial.diffuseMap();
					} else {
						return reinterpret_cast<milk::graphics::Texture2d*>(nullptr);
					}
				},
				[](const PassContext& context) {
					assert(context.material->type() == PhongMaterial::TYPE);
					const auto& phongMaterial = reinterpret_cast<const PhongMaterial&>(*context.material);
					return phongMaterial.diffuseMapSampler();
				},
				milk::graphics::ShaderType::PIXEL,
				0,
				0
				);
			resources.insert(std::make_pair(0, resource));
		}

		pixelShader = std::make_unique<PixelShader>(
			*binaryShader,
			std::move(sceneData),
			std::move(actorData),
			std::move(groupData),
			std::move(resources)
			);
	}

	return std::make_unique<Pass>(std::move(inputLayout), std::move(vertexShader), std::move(pixelShader));
}
