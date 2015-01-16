#ifndef _COCONUT_PULP_RENDERER_MATERIAL_HPP_
#define _COCONUT_PULP_RENDERER_MATERIAL_HPP_

#include <string>

#include "milk/graphics/Device.hpp"
#include "milk/graphics/InputLayoutDescription.hpp"

#include "milk/utils/MakePointerDefinitionsMacro.hpp"

#include "shader/ShaderSet.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

struct RenderingContext;

class Material {
public:

	typedef std::string Id;

	Material(milk::graphics::Device& graphicsDevice, shader::ShaderSetSharedPtr shaderSet);

	void bind(milk::graphics::Device& graphicsDevice, RenderingContext renderingContext);

	milk::graphics::InputLayoutDescriptionSharedPtr inputLayout() {
		return shaderSet_->inputLayout();
	}

private:

	shader::ShaderSetSharedPtr shaderSet_;

};

MAKE_POINTER_DEFINITIONS(Material);

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_MATERIAL_HPP_ */
