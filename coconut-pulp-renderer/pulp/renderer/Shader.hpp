#ifndef _COCONUT_PULP_RENDERER_SHADER_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_HPP_

#include "milk/graphics/Shader.hpp"
#include "milk/graphics/Device.hpp"
#include "milk/utils/MakePointerDefinitionsMacro.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class Shader {
public:

	Shader(milk::graphics::ShaderSharedPtr wrappedShader) :
		wrappedShader_(wrappedShader)
	{
	}

	void bind(milk::graphics::Device& device) {
		update(device);
		wrappedShader_->bind(device);
	}

protected:

	virtual ~Shader() {
	}

	virtual void update(milk::graphics::Device& device) = 0;

private:

	milk::graphics::ShaderSharedPtr wrappedShader_;

};

MAKE_POINTER_DEFINITIONS(Shader);

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_HPP_ */
