#ifndef _COCONUT_PULP_RENDERER_SHADER_CONSTANTBUFFER_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_CONSTANTBUFFER_HPP_

#include <memory>

#include "coconut/milk/graphics/ConstantBuffer.hpp"
#include "coconut/milk/graphics/Renderer.hpp"

#include "Parameter.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace shader {

template <class... UpdateArguments>
class ConstantBuffer {
public:

	ConstantBuffer(
		milk::graphics::Renderer& renderer,
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
				false
				)
			),
		parameter_(std::move(parameter))
	{
	}

	void update(milk::graphics::CommandList& commandList, const UpdateArguments&... updateArguments) { // TODO: why doesn't perfect forwarding work here?
		auto lockedData = commandList.lock(buffer_, milk::graphics::Buffer::LockPurpose::WRITE_DISCARD);
		parameter_->update(lockedData.get(), updateArguments...);
	}

private:

	milk::graphics::ShaderType shaderType_;

	size_t slot_;

	milk::graphics::ConstantBuffer buffer_;

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
