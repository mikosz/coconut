#ifndef _COCONUT_PULP_RENDERER_SHADER_SHADERSET_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_SHADERSET_HPP_

#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"
#include "coconut/milk/graphics/InputLayoutDescription.hpp"
#include "coconut/milk/graphics/Device.hpp"

#include "../RenderingContext.hpp"
#include "Shader.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace shader {

class ShaderSet {
public:

	ShaderSet(
		ShaderSharedPtr vertexShader,
		ShaderSharedPtr pixelShader,
		milk::graphics::InputLayoutDescriptionSharedPtr inputLayout
		) :
		vertexShader_(vertexShader),
		pixelShader_(pixelShader),
		inputLayout_(inputLayout)
	{
	}

	void bind(milk::graphics::Device& graphicsDevice, RenderingContext renderingContext);

	milk::graphics::InputLayoutDescriptionSharedPtr inputLayout() {
		return inputLayout_;
	}

private:

	ShaderSharedPtr vertexShader_;
		
	ShaderSharedPtr pixelShader_;
	
	milk::graphics::InputLayoutDescriptionSharedPtr inputLayout_;

};

MAKE_POINTER_DEFINITIONS(ShaderSet);

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_SHADERSET_HPP_ */
