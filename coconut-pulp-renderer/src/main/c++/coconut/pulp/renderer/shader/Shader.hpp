#ifndef _COCONUT_PULP_RENDERER_SHADER_SHADER_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_SHADER_HPP_

#include <unordered_map>

#include "coconut/milk/graphics/Device.hpp"
#include "coconut/milk/graphics/Shader.hpp"
#include "coconut/milk/graphics/ShaderType.hpp"

#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

#include "ActorParameter.hpp"
#include "SceneParameter.hpp"
#include "MaterialParameter.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

struct RenderingContext;

namespace shader {

class Resource;
MAKE_POINTER_DEFINITIONS(Resource);

class Shader {
public:

	typedef std::unordered_map<size_t, SceneParameterSharedPtr> SceneParameters;

	typedef std::unordered_map<size_t, ActorParameterSharedPtr> ActorParameters;

	typedef std::unordered_map<size_t, MaterialParameterSharedPtr> MaterialParameters;

	typedef std::unordered_map<size_t, ResourceSharedPtr> Resources;

	Shader(
		milk::graphics::ShaderSharedPtr binaryShader,
		milk::graphics::ShaderType shaderType,
		SceneParameters sceneParameters,
		ActorParameters actorParameters,
		MaterialParameters materialParameters,
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

	SceneParameters sceneParameters_;

	ActorParameters actorParameters_;

	MaterialParameters materialParameters_;

	Resources resources_;

};

MAKE_POINTER_DEFINITIONS(Shader);

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_SHADER_HPP_ */
