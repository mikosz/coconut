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

	virtual ~Resource() = default;

	virtual void bind(DrawCommand& drawCommand, const PassContext& context) = 0;

};

CCN_MAKE_POINTER_DEFINITIONS(Resource);

class TextureResource : public Resource {
public:

	// TODO: pointer?
	using TextureCallback = std::function<milk::graphics::Texture* (const PassContext&)>;

	TextureResource(
		TextureCallback callback,
		milk::graphics::ShaderType shaderType,
		size_t slot
		) :
		callback_(callback),
		stage_(shaderType),
		slot_(slot)
	{
	}

	void bind(DrawCommand& drawCommand, const PassContext& context) override {
		auto* texture = callback_(context);
		if (texture) {
			drawCommand.addTexture(texture, stage_, slot_);
		}
	}
	
private:

	TextureCallback callback_;

	milk::graphics::ShaderType stage_;

	size_t slot_;

};

class SamplerResource : public Resource {
public:

	// TODO: pointer?
	using SamplerCallback = std::function<milk::graphics::Sampler* (const PassContext&)>;

	SamplerResource(
		SamplerCallback callback,
		milk::graphics::ShaderType shaderType,
		size_t slot
		) :
		callback_(callback),
		stage_(shaderType),
		slot_(slot)
	{
	}

	void bind(DrawCommand& drawCommand, const PassContext& context) override {
		auto* sampler = callback_(context);
		if (sampler) {
			drawCommand.addSampler(*sampler, stage_, slot_);
		}
	}

	
private:

	SamplerCallback callback_;

	milk::graphics::ShaderType stage_;

	size_t slot_;

};

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_RESOURCE_HPP_ */
