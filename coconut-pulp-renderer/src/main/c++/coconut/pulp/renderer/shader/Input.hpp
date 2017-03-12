#ifndef _COCONUT_PULP_RENDERER_SHADER_INPUT_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_INPUT_HPP_

#include <functional>
#include <vector>

#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

#include "coconut/milk/graphics/InputLayout.hpp"
#include "coconut/milk/graphics/Renderer.hpp"

#include "coconut/pulp/mesh/Submesh.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

namespace shader {

class Input {
public:

	using SlotType = milk::graphics::InputLayout::SlotType;

	struct Element : public milk::graphics::InputLayout::Element { // TODO: unsure about this api

		using WriteFunc =
			std::function<void (void* buffer, const void* input)>; // TODO: input type? vertex or instance...

		Element(
			std::string semantic,
			size_t semanticIndex,
			milk::graphics::PixelFormat format,
			SlotType inputSlotType,
			size_t instanceDataStepRate,
			WriteFunc writeFunc
			);

		WriteFunc writeFunc;

	};

	using Elements = std::vector<Element>;

	Input(milk::graphics::Renderer& graphicsRenderer, Elements elements);

	size_t vertexSize() const;

	void writeVertex(void* buffer, const void* input) const;

	milk::graphics::InputLayout& layout() {
		return layout_;
	}

private:

	Elements elements_;

	milk::graphics::InputLayout layout_;

};

CCN_MAKE_POINTER_DEFINITIONS(Input);

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_INPUT_HPP_ */
