#ifndef _COCONUT_PULP_RENDERER_SHADER_CONSTANTBUFFER_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_CONSTANTBUFFER_HPP_

#include <memory>
#include <vector>
#include <numeric>

#include "coconut/milk/graphics/ConstantBuffer.hpp"

#include "../DrawCommand.hpp"
#include "Parameter.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace shader {

template <class UpdateArgument>
class ConstantBuffer {
public:

	using Parameters = std::vector<ParameterSharedPtr>;

	ConstantBuffer(
		milk::graphics::Renderer& renderer,
		milk::graphics::ShaderType shaderType,
		size_t size,
		size_t slot,
		Parameters parameters
		) :
		stage_(shaderType),
		slot_(slot),
		buffer_(
			renderer,
			milk::graphics::Buffer::Configuration(
				size,
				0,
				true,
				false,
				false
				)
			),
		data_(size),
		parameters_(std::move(parameters))
	{
		// TODO: check parameter input type against UpdateArgument
		const auto totalParameterSize = std::accumulate(
			parameters_.begin(),
			parameters_.end(),
			static_cast<size_t>(0),
			[](size_t total, ParameterSharedPtr parameter) { return total + parameter->size(); }
			);

		if (totalParameterSize > size) {
			throw "";
#pragma message("!!! TODO: exception") // TODO
		}
	}

	void bind(DrawCommand& drawCommand, const UpdateArgument& updateArgument) {
		for (const auto parameter : parameters_) {
			parameter->update(data_.data(), &updateArgument);
		}

		drawCommand.addConstantBufferData(&buffer_, data_.data(), data_.size(), stage_, slot_);
	}

private:

	milk::graphics::ShaderType stage_;

	size_t slot_;

	milk::graphics::ConstantBuffer buffer_;

	std::vector<std::uint8_t> data_;

	Parameters parameters_;

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
