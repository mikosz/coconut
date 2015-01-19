#include "Shader.hpp"

#include "../Scene.hpp"
#include "../Actor.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;
using namespace coconut::pulp::renderer::shader;

void Shader::bind(milk::graphics::Device& graphicsDevice, const RenderingContext& renderingContext) {
	for (auto sceneParameter : sceneParameters_) {
		sceneParameter.second->update(graphicsDevice, *renderingContext.scene); // TODO: update conditionally (if changed since last update)
		sceneParameter.second->bind(graphicsDevice, sceneParameter.first, shaderType_);
	}

	for (auto actorParameter : actorParameters_) {
		actorParameter.second->update(graphicsDevice, *renderingContext.actor); // TODO: update conditionally (if changed since last update)
		actorParameter.second->bind(graphicsDevice, actorParameter.first, shaderType_);
	}

	for (auto materialParameter : materialParameters_) {
		materialParameter.second->update(graphicsDevice, *renderingContext.material); // TODO: update conditionally (if changed since last update)
		materialParameter.second->bind(graphicsDevice, materialParameter.first, shaderType_);
	}

	binaryShader_->bind(graphicsDevice);
}
