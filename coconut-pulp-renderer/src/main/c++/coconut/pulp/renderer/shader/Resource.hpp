#ifndef _COCONUT_PULP_RENDERER_SHADER_RESOURCE_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_RESOURCE_HPP_

#include <functional>

#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

#include "coconut/milk/graphics/Device.hpp"
#include "coconut/milk/graphics/ShaderResource.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

struct RenderingContext;

namespace shader {

class Resource {
public:

	using Callback = std::function<milk::graphics::ShaderResourceSharedPtr (milk::graphics::Device&, const RenderingContext&)>;

	Resource(Callback callback) :
		callback_(callback)
	{
	}

	void update(milk::graphics::Device& graphicsDevice, const RenderingContext& context) {
		resource_ = callback_(graphicsDevice, context);
	}

	void bind(
		milk::graphics::Device& graphicsDevice,
		size_t slot,
		milk::graphics::ShaderType shaderType
		) {
		// TODO: check if resource not null...
		resource_->bind(graphicsDevice, shaderType, slot);
	}

private:

	Callback callback_;

	milk::graphics::ShaderResourceSharedPtr resource_;

};

MAKE_POINTER_DEFINITIONS(Resource);

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_RESOURCE_HPP_ */
