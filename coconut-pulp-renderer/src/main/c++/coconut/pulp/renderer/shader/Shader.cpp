#include "Shader.hpp"

#include "../PassContext.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;
using namespace coconut::pulp::renderer::shader;

template <class GraphicsShaderType>
detail::Shader<GraphicsShaderType>::Shader(
	GraphicsShaderType shaderData,
	SceneData sceneData,
	ActorData actorData,
	MaterialData materialData,
	Resources /*resources*/
	) :
	shaderData_(shaderData),
	sceneData_(std::move(sceneData)),
	actorData_(std::move(actorData)),
	materialData_(std::move(materialData))//,
	// resources_(std::move(resources))
{
}

template <class GraphicsShaderType>
void detail::Shader<GraphicsShaderType>::bind(
	DrawCommand& drawCommand,
	const PassContext& PassContext
	) const
{
	for (auto buffer : sceneData_) {
		buffer->bind(drawCommand, *PassContext.scene); // TODO: update conditionally (if changed since last update)
	}

	for (auto buffer : actorData_) {
		buffer->bind(drawCommand, *PassContext.actor); // TODO: update conditionally (if changed since last update)
	}

	for (auto buffer : materialData_) {
		buffer->bind(drawCommand, *PassContext.material); // TODO: update conditionally (if changed since last update)
	}

/*	for (auto resource : resources_) {
		resource.second->bind(drawCommand, PassContext);
	} */
}

template class detail::Shader<milk::graphics::VertexShader>;
template class detail::Shader<milk::graphics::PixelShader>;
