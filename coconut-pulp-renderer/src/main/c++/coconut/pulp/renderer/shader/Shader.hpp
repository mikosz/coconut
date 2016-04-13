#ifndef _COCONUT_PULP_RENDERER_SHADER_SHADER_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_SHADER_HPP_

#include <vector>
#include <unordered_map>

#include "coconut/milk/graphics/Shader.hpp"

#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

#include "coconut/pulp/renderer/material/Material.hpp"

#include "ConstantBuffer.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

struct RenderingContext;
class Actor;
class Scene;

namespace shader {

class Resource;
CCN_MAKE_POINTER_DEFINITIONS(Resource);

namespace detail {

template <class GraphicsShaderType>
class Shader {
public:

	using SceneData = std::vector<ConstantBufferSharedPtr<Scene>>;

	using ActorData = std::vector<ConstantBufferSharedPtr<Actor>>;

	using MaterialData = std::vector<ConstantBufferSharedPtr<material::Material>>;

	using Resources = std::unordered_map<size_t, ResourceSharedPtr>;

	Shader(
		GraphicsShaderType shaderData,
		SceneData sceneData,
		ActorData actorData,
		MaterialData materialData,
		Resources resources
		);

	void update(
		milk::graphics::Device& graphicsDevice,
		const RenderingContext& renderingContext
		) const;

	GraphicsShaderType& shaderData() {
		return shaderData_;
	}

private:

	GraphicsShaderType shaderData_;

	SceneData sceneData_;

	ActorData actorData_;

	MaterialData materialData_;

	Resources resources_;

};

} // namespace detail

using VertexShader = detail::Shader<milk::graphics::VertexShader>;
using PixelShader = detail::Shader<milk::graphics::PixelShader>;

CCN_MAKE_POINTER_DEFINITIONS(VertexShader);
CCN_MAKE_POINTER_DEFINITIONS(PixelShader);

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_SHADER_HPP_ */
