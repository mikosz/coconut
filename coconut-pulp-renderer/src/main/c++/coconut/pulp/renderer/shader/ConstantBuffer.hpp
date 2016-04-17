#ifndef _COCONUT_PULP_RENDERER_SHADER_CONSTANTBUFFER_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_CONSTANTBUFFER_HPP_

#include <memory>
#include <vector>

#include "coconut/milk/graphics/ConstantBuffer.hpp"

#include "../DrawCommand.hpp"
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
		stage_(shaderType),
		slot_(slot),
		buffer_(
			renderer,
			milk::graphics::Buffer::Configuration(
				parameter->size(),
				0,
				true,
				false,
				false
				)
			),
		data_(parameter->size()),
		parameter_(std::move(parameter))
	{
	}

	void bind(DrawCommand& drawCommand, const UpdateArguments&... updateArguments) { // TODO: why doesn't perfect forwarding work here?
		parameter_->update(data_.data(), updateArguments...);
		drawCommand.addConstantBufferData(&buffer_, data_.data(), data_.size(), stage_, slot_);
	}

private:

	milk::graphics::ShaderType stage_;

	size_t slot_;

	milk::graphics::ConstantBuffer buffer_;

	std::vector<std::uint8_t> data_;

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
