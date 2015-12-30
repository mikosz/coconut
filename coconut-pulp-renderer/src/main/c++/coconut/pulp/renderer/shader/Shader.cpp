#include "Shader.hpp"

#include "coconut/pulp/renderer/Actor.hpp"
#include "coconut/pulp/renderer/Scene.hpp"
#include "coconut/pulp/renderer/RenderingContext.hpp"

#include "Resource.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;
using namespace coconut::pulp::renderer::shader;

Shader::Shader(
	milk::graphics::ShaderSharedPtr binaryShader,
	milk::graphics::ShaderType shaderType,
	SceneData sceneData,
	ActorData actorData,
	MaterialData materialData,
	Resources resources
	) :
	binaryShader_(binaryShader),
	shaderType_(shaderType),
	sceneData_(std::move(sceneData)),
	actorData_(std::move(actorData)),
	materialData_(std::move(materialData)),
	resources_(std::move(resources))
{
}

void Shader::bind(milk::graphics::Device& graphicsDevice, const RenderingContext& renderingContext) const {
	for (auto buffer : sceneData_) {
		buffer->update(graphicsDevice, *renderingContext.scene); // TODO: update conditionally (if changed since last update)
		buffer->bind(graphicsDevice);
	}

	for (auto buffer : actorData_) {
		buffer->update(graphicsDevice, *renderingContext.actor); // TODO: update conditionally (if changed since last update)
		buffer->bind(graphicsDevice);
	}

	for (auto buffer : materialData_) {
		buffer->update(graphicsDevice, *renderingContext.material); // TODO: update conditionally (if changed since last update)
		buffer->bind(graphicsDevice);
	}

	for (auto resource : resources_) {
		resource.second->update(graphicsDevice, renderingContext);
		resource.second->bind(graphicsDevice, resource.first, shaderType_);
	}

	binaryShader_->bind(graphicsDevice);
}
