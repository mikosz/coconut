#include "ShaderFactory.hpp"

#include <fstream>
#include <stdexcept>
#include <vector>

#include <boost/filesystem.hpp>

#include "milk/graphics/VertexShader.hpp"
#include "milk/graphics/PixelShader.hpp"
#include "milk/graphics/FlexibleInputLayoutDescription.hpp"

#include "milk/math/Matrix.hpp"

#include "CallbackUpdateableParameter.hpp"
#include "Shader.hpp"
#include "../Actor.hpp"
#include "../Scene.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;
using namespace coconut::pulp::renderer::shader;

ShaderSharedPtr ShaderFactory::createShader(milk::graphics::Device& graphicsDevice, ShaderId shaderId) {
	milk::graphics::ShaderSharedPtr binaryShader;
	milk::graphics::Buffer::ShaderType shaderType;

	Shader::SceneParameters sceneParameters;
	Shader::ActorParameters actorParameters;

	if (shaderId == "VS") {
		shaderType = milk::graphics::Buffer::ShaderType::VERTEX;

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

		binaryShader.reset(
			new milk::graphics::VertexShader(
				graphicsDevice,
				&vdata.front(),
				vdata.size(),
				inputLayoutDesc
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

	} else if (shaderId == "PS") {
		shaderType = milk::graphics::Buffer::ShaderType::PIXEL;

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

		binaryShader.reset(new milk::graphics::PixelShader(graphicsDevice, &pdata.front(), pdata.size()));
	} else {
		throw std::runtime_error("Unknown shader id");
	}

	ShaderSharedPtr shader(new Shader(binaryShader, shaderType, sceneParameters, actorParameters));

	return shader;
}
