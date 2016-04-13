#include "Shader.hpp"

#include "coconut/pulp/renderer/Actor.hpp"
#include "coconut/pulp/renderer/Scene.hpp"
#include "coconut/pulp/renderer/RenderingContext.hpp"

#include "Resource.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;
using namespace coconut::pulp::renderer::shader;

template <class GraphicsShaderType>
detail::Shader<GraphicsShaderType>::Shader(
	GraphicsShaderType binaryShader,
	SceneData sceneData,
	ActorData actorData,
	MaterialData materialData,
	Resources resources
	) :
	binaryShader_(binaryShader),
	sceneData_(std::move(sceneData)),
	actorData_(std::move(actorData)),
	materialData_(std::move(materialData)),
	resources_(std::move(resources))
{
}

template <class GraphicsShaderType>
void detail::Shader<GraphicsShaderType>::update(milk::graphics::Device& graphicsDevice, const RenderingContext& renderingContext) const {
	for (auto buffer : sceneData_) {
		buffer->update(graphicsDevice, *renderingContext.scene); // TODO: update conditionally (if changed since last update)
	}

	for (auto buffer : actorData_) {
		buffer->update(graphicsDevice, *renderingContext.actor); // TODO: update conditionally (if changed since last update)
	}

	for (auto buffer : materialData_) {
		buffer->update(graphicsDevice, *renderingContext.material); // TODO: update conditionally (if changed since last update)
	}

	for (auto resource : resources_) {
		resource.second->update(graphicsDevice, renderingContext);
	}
}
