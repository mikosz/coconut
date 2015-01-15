#ifndef _COCONUT_PULP_RENDERER_DRAWGROUP_HPP_
#define _COCONUT_PULP_RENDERER_DRAWGROUP_HPP_

#include "milk/graphics/Buffer.hpp"
#include "milk/graphics/PrimitiveTopology.hpp"

#include "Material.hpp"
#include "RenderingContext.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class DrawGroup {
public:

	struct Data {

		size_t vertexCount;

		size_t indexCount;

	};

	DrawGroup(MaterialSharedPtr material);

	void render(milk::graphics::Device& graphicsDevice, RenderingContext renderingContext);

private:

	MaterialSharedPtr material_;

	milk::graphics::Buffer vertexBuffer_;

	milk::graphics::Buffer indexBuffer_;

	size_t indexCount_;

	milk::graphics::PrimitiveTopology primitiveTopology_;

};

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_DRAWGROUP_HPP_ */
