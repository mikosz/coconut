#ifndef _COCONUT_PULP_RENDERER_SHADER_CONSTANTBUFFER_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_CONSTANTBUFFER_HPP_

#include <cassert>
#include <memory>
#include <vector>
#include <numeric>

#include <coconut-tools/exceptions/LogicError.hpp>

#include "coconut/milk/graphics/ConstantBuffer.hpp"

#include "../DrawCommand.hpp"
#include "Parameter.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace shader {

class ConstantBuffer {
public:

	using Parameters = std::vector<Parameter>;

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
	}

	void bind(DrawCommand& drawCommand, const Properties& properties) {
		for (const auto parameter : parameters_) {
			auto* endPtr = parameter.write(data_.data(), properties);
			if (endPtr > (&data_.back() + 1)) {
				throw coconut_tools::exceptions::LogicError("Data written past end of buffer");
			}
		}

		// TODO: unnecessarily keeping data in data_ AND in DrawCommand
		drawCommand.addConstantBufferData(&buffer_, data_.data(), data_.size(), stage_, slot_);
	}

private:

	milk::graphics::ShaderType stage_;

	size_t slot_;

	milk::graphics::ConstantBuffer buffer_;

	std::vector<std::uint8_t> data_;

	Parameters parameters_;

};

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_CONSTANTBUFFER_HPP_ */
