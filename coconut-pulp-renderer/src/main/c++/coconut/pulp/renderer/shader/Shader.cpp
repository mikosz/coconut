#include "Shader.hpp"

#include "Resource.hpp"

#include "../Scene.hpp"
#include "../Actor.hpp"
#include "../RenderingContext.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;
using namespace coconut::pulp::renderer::shader;

Shader::Shader(
	milk::graphics::ShaderSharedPtr binaryShader,
	milk::graphics::ShaderType shaderType,
	SceneParameters sceneParameters,
	ActorParameters actorParameters,
	MaterialParameters materialParameters,
	Resources resources
	) :
	binaryShader_(binaryShader),
	shaderType_(shaderType),
	sceneParameters_(std::move(sceneParameters)),
	actorParameters_(std::move(actorParameters)),
	materialParameters_(std::move(materialParameters)),
	resources_(std::move(resources))
{
}

void Shader::bind(milk::graphics::Device& graphicsDevice, const RenderingContext& renderingContext) const {
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

	for (auto resource : resources_) {
		resource.second->update(graphicsDevice, renderingContext);
		resource.second->bind(graphicsDevice, resource.first, shaderType_);
	}

	binaryShader_->bind(graphicsDevice);
}
