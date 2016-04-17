#ifndef _COCONUT_PULP_RENDERER_SHADER_RESOURCE_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_RESOURCE_HPP_

#include <functional>

#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

#include "coconut/milk/graphics/Texture.hpp"

#include "../DrawCommand.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

struct RenderingContext;

namespace shader {

class Resource {
public:

	// TODO: pointer?
	using Callback = std::function<milk::graphics::Texture* (const RenderingContext&)>;

	Resource(Callback callback, milk::graphics::ShaderType shaderType, size_t slot) :
		callback_(callback),
		stage_(shaderType),
		slot_(slot)
	{
	}

	void bind(DrawCommand& drawCommand, const RenderingContext& context) {
		drawCommand.addTexture(callback_(context), stage_, slot_);
	}

private:

	Callback callback_;

	milk::graphics::ShaderType stage_;

	size_t slot_;

};

CCN_MAKE_POINTER_DEFINITIONS(Resource);

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_RESOURCE_HPP_ */
