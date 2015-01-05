#ifndef _COCONUT_PULP_RENDERER_SHADER_SHADER_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_SHADER_HPP_

#include <unordered_map>

#include "milk/graphics/Device.hpp"
#include "milk/graphics/Shader.hpp"

#include "milk/utils/MakePointerDefinitionsMacro.hpp"

#include "ActorParameter.hpp"
#include "SceneParameter.hpp"
#include "../RenderingContext.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace shader {

class Shader {
public:

	typedef std::unordered_map<size_t, SceneParameterSharedPtr> SceneParameters;

	typedef std::unordered_map<size_t, ActorParameterSharedPtr> ActorParameters;

	Shader(
		milk::graphics::ShaderSharedPtr binaryShader,
		milk::graphics::Buffer::ShaderType shaderType,
		const SceneParameters& sceneParameters,
		const ActorParameters& actorParameters
		) :
		binaryShader_(binaryShader),
		shaderType_(shaderType),
		sceneParameters_(sceneParameters),
		actorParameters_(actorParameters)
	{
	}

	void bind(
		milk::graphics::Device& graphicsDevice,
		const RenderingContext& renderingContext
		);

private:

	milk::graphics::ShaderSharedPtr binaryShader_;

	milk::graphics::Buffer::ShaderType shaderType_;

	SceneParameters sceneParameters_;

	ActorParameters actorParameters_;

};

MAKE_POINTER_DEFINITIONS(Shader);

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_SHADER_HPP_ */
