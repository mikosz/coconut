#ifndef _COCONUT_PULP_RENDERER_SHADER_INPUT_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_INPUT_HPP_

#include <vector>

#include "coconut/milk/graphics/InputLayout.hpp"
#include "Parameter.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class Actor;

namespace shader {

class Input {
public:

	using Parameters = std::vector<Parameter>;

	Input(
		milk::graphics::InputLayout layout,
		Parameters perVertexParameters,
		Parameters perInstanceParameters
		) noexcept;

	size_t vertexSize() const noexcept;

	void* writeVertex(void* buffer, const Properties& properties) const;

	size_t instanceSize() const noexcept;

	void* writeInstance(void* buffer, const Properties& properties) const;

	const milk::graphics::InputLayout& layout() const {
		return layout_;
	}

private:

	milk::graphics::InputLayout layout_;

	Parameters perVertexParameters_;

	Parameters perInstanceParameters_;

};

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_INPUT_HPP_ */
