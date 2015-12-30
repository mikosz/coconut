#ifndef _COCONUT_PULP_RENDERER_SHADER_CONSTANTBUFFER_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_CONSTANTBUFFER_HPP_

#include <memory>

#include "coconut/milk/graphics/Buffer.hpp"
#include "coconut/milk/graphics/Device.hpp"

#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

#include "Parameter.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace shader {

template <class... UpdateArguments>
class ConstantBuffer {
public:

	ConstantBuffer(
		milk::graphics::Device& graphicsDevice,
		milk::graphics::ShaderType shaderType,
		size_t slot,
		std::unique_ptr<Parameter<UpdateArguments...>> parameter
		) :
		shaderType_(shaderType),
		slot_(slot),
		buffer_(
			graphicsDevice,
			milk::graphics::Buffer::Configuration(
				parameter->size(),
				0,
				true,
				false,
				false,
				milk::graphics::Buffer::CreationPurpose::CONSTANT_BUFFER
				)
			),
		parameter_(std::move(parameter))
	{
	}

	void update(milk::graphics::Device& graphicsDevice, const UpdateArguments&... updateArguments) { // TODO: why doesn't perfect forwarding work here?
		auto lockedData = buffer_.lock(graphicsDevice, milk::graphics::Buffer::LockPurpose::WRITE_DISCARD);
		parameter_->update(lockedData.data, updateArguments...);
	}

	void bind(milk::graphics::Device& graphicsDevice) {
		buffer_.bind(graphicsDevice, shaderType_, slot_);
	}

private:

	milk::graphics::ShaderType shaderType_;

	size_t slot_;

	milk::graphics::Buffer buffer_;

	std::unique_ptr<Parameter<UpdateArguments...>> parameter_;

};

template <class... UpdateArguments>
using ConstantBufferUniquePtr = std::unique_ptr<ConstantBuffer<UpdateArguments...>>;

template <class... UpdateArguments>
using ConstantBufferSharedPtr = std::shared_ptr<ConstantBuffer<UpdateArguments...>>;

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_CONSTANTBUFFER_HPP_ */
