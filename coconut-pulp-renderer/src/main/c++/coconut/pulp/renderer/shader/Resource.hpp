#ifndef _COCONUT_PULP_RENDERER_SHADER_RESOURCE_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_RESOURCE_HPP_

#include <functional>

#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

#include "coconut/milk/graphics/Texture.hpp"

#include "../DrawCommand.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class PassContext;

namespace shader {

class Resource {
public:

	// TODO: pointer?
	using TextureCallback = std::function<milk::graphics::Texture* (const PassContext&)>;

	using SamplerCallback = std::function<milk::graphics::Sampler (const PassContext&)>;

	Resource(
		TextureCallback textureCallback,
		SamplerCallback samplerCallback,
		milk::graphics::ShaderType shaderType,
		size_t textureSlot,
		size_t samplerSlot
		) :
		textureCallback_(textureCallback),
		samplerCallback_(samplerCallback),
		stage_(shaderType),
		textureSlot_(textureSlot),
		samplerSlot_(samplerSlot)
	{
	}

	void bind(DrawCommand& drawCommand, const PassContext& context) {
		drawCommand.addTexture(textureCallback_(context), stage_, textureSlot_);
		drawCommand.addSampler(samplerCallback_(context), stage_, samplerSlot_);
	}

private:

	TextureCallback textureCallback_;

	SamplerCallback samplerCallback_;

	milk::graphics::ShaderType stage_;

	size_t textureSlot_;

	size_t samplerSlot_;

};

CCN_MAKE_POINTER_DEFINITIONS(Resource);

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_RESOURCE_HPP_ */
