#include "Parameter.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;
using namespace coconut::pulp::renderer::shader;

Parameter::Parameter(
	milk::graphics::Device& graphicsDevice,
	size_t size
	) :
	buffer_(
		graphicsDevice,
		milk::graphics::Buffer::Configuration(
			size,
			0,
			true,
			false,
			false,
			milk::graphics::Buffer::CreationPurpose::CONSTANT_BUFFER
			)
		)
{
}

void Parameter::bind(
	milk::graphics::Device& graphicsDevice,
	size_t slot,
	milk::graphics::ShaderType shaderType
	) {
	buffer_.bind(graphicsDevice, shaderType, slot);
}
