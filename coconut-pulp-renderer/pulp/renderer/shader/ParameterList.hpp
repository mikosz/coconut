#ifndef _COCONUT_PULP_RENDERER_SHADER_PARAMETERLIST_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_PARAMETERLIST_HPP_

#include <vector>

#include "milk/graphics/Buffer.hpp"
#include "ParameterId.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace shader {

class ParameterWriter;

class ParameterList {
public:

	struct BufferDescription {

		typedef std::vector<ParameterId> ParameterIds;

		ParameterIds parameterIds;

	};

	typedef std::vector<BufferDescription> BufferDescriptions;

	typedef std::vector<milk::graphics::BufferSharedPtr> Buffers;

	void add(milk::graphics::Device& device, const BufferDescription& bufferDescription);

	void bind(
		milk::graphics::Device& device,
		ParameterWriter& writer,
		milk::graphics::Buffer::ShaderType shaderType
		);

private:

	BufferDescriptions bufferDescriptions_;

	Buffers buffers_;

};

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_PARAMETERLIST_HPP_ */
