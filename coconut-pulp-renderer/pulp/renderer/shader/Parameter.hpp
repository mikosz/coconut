#ifndef _COCONUT_PULP_RENDERER_SHADER_PARAMETER_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_PARAMETER_HPP_

#include "milk/graphics/Buffer.hpp"
#include "milk/graphics/Device.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

namespace shader {

class Parameter {
public:

	Parameter(milk::graphics::Device& graphicsDevice, size_t size);

	void bind(
		milk::graphics::Device& graphicsDevice,
		size_t slot,
		milk::graphics::Buffer::ShaderType shaderType
		);

	milk::graphics::Buffer& buffer() {
		return buffer_;
	}

private:

	milk::graphics::Buffer buffer_;

};

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_PARAMETER_HPP_ */
