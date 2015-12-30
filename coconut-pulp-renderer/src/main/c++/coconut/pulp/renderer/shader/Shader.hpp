#ifndef _COCONUT_PULP_RENDERER_SHADER_SHADER_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_SHADER_HPP_

#include <vector>
#include <unordered_map>

#include "coconut/milk/graphics/Device.hpp"
#include "coconut/milk/graphics/Shader.hpp"
#include "coconut/milk/graphics/ShaderType.hpp"

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
MAKE_POINTER_DEFINITIONS(Resource);

class Shader {
public:

	using SceneData = std::vector<ConstantBufferSharedPtr<Scene>>;

	using ActorData = std::vector<ConstantBufferSharedPtr<Actor>>;

	using MaterialData = std::vector<ConstantBufferSharedPtr<material::Material>>;

	using Resources = std::unordered_map<size_t, ResourceSharedPtr>;

	Shader(
		milk::graphics::ShaderSharedPtr binaryShader,
		milk::graphics::ShaderType shaderType,
		SceneData sceneData,
		ActorData actorData,
		MaterialData materialData,
		Resources resources
		);

	void bind(
		milk::graphics::Device& graphicsDevice,
		const RenderingContext& renderingContext
		) const;

	milk::graphics::ShaderSharedPtr binaryShader() {
		return binaryShader_;
	}

private:

	milk::graphics::ShaderSharedPtr binaryShader_;

	milk::graphics::ShaderType shaderType_;

	SceneData sceneData_;

	ActorData actorData_;

	MaterialData materialData_;

	Resources resources_;

};

MAKE_POINTER_DEFINITIONS(Shader);

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_SHADER_HPP_ */
