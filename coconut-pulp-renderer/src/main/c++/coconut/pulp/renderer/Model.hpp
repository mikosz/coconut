#ifndef _COCONUT_PULP_RENDERER_MODEL_HPP_
#define _COCONUT_PULP_RENDERER_MODEL_HPP_

#include <vector>

#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

#include "coconut/pulp/model/Data.hpp"

#include "RenderingContext.hpp"
#include "DrawGroup.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class Model {
public:

	Model(
		const model::Data& data,
		milk::graphics::Device& graphicsDevice,
		const milk::graphics::InputLayoutDescription& inputLayoutDescription
		);

	void render(milk::graphics::Device& graphicsDevice, RenderingContext renderingContext);

private:

	// TODO: make draw groups using the same materials be drawn one after another to avoid resetting constant buffers
	using DrawGroups = std::vector<DrawGroupSharedPtr>;

	DrawGroups drawGroups_;

	friend class ModelDataListener;

};

CCN_MAKE_POINTER_DEFINITIONS(Model);

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_MODEL_HPP_ */
