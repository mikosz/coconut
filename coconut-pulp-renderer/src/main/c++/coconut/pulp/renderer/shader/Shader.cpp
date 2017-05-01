#include "Shader.hpp"

#include "../PassContext.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;
using namespace coconut::pulp::renderer::shader;
using namespace coconut::pulp::renderer::shader::detail;

template <class GraphicsShaderType>
Shader<GraphicsShaderType>::Shader(
	GraphicsShaderType shaderData,
	SceneData sceneData,
	ActorData actorData,
	MaterialData materialData,
	Resources resources
	) :
	shaderData_(shaderData),
	sceneData_(std::move(sceneData)),
	actorData_(std::move(actorData)),
	materialData_(std::move(materialData)),
	resources_(std::move(resources))
{
}

template <class GraphicsShaderType>
void Shader<GraphicsShaderType>::bind(DrawCommand& drawCommand, const PassContext& passContext) const {
	for (auto buffer : sceneData_) {
		buffer->bind(drawCommand, *passContext.scene); // TODO: update conditionally (if changed since last update)
	}

	for (auto buffer : actorData_) {
		buffer->bind(drawCommand, *passContext.actor); // TODO: update conditionally (if changed since last update)
	}

	for (auto buffer : materialData_) {
		buffer->bind(drawCommand, *passContext.material); // TODO: update conditionally (if changed since last update)
	}

	for (auto resource : resources_) {
		resource->bind(drawCommand, passContext);
	}
}

template class Shader<milk::graphics::VertexShader>;
template class Shader<milk::graphics::GeometryShader>;
template class Shader<milk::graphics::HullShader>;
template class Shader<milk::graphics::DomainShader>;
template class Shader<milk::graphics::PixelShader>;
