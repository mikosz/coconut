#ifndef _COCONUT_PULP_RENDERER_DRAWGROUP_HPP_
#define _COCONUT_PULP_RENDERER_DRAWGROUP_HPP_

#include <vector>

#include "milk/graphics/Buffer.hpp"
#include "milk/graphics/PrimitiveTopology.hpp"
#include "milk/graphics/VertexInterface.hpp"

#include "milk/utils/MakePointerDefinitionsMacro.hpp"

#include "Material.hpp"
#include "RenderingContext.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class DrawGroup {
public:

	struct Data {

		milk::graphics::PrimitiveTopology primitiveTopology;

		MaterialSharedPtr material;

		milk::graphics::InputLayoutDescriptionSharedPtr inputLayout;

		std::vector<milk::graphics::VertexInterfaceSharedPtr> vertices;

		std::vector<size_t> indices;

		void reset() {
			material.reset();
			inputLayout.reset();
			vertices.clear();
			indices.clear();
		}

	};

	DrawGroup(milk::graphics::Device& graphicsDevice, const Data& data);

	void render(milk::graphics::Device& graphicsDevice, RenderingContext renderingContext);

private:

	MaterialSharedPtr material_;

	milk::graphics::Buffer vertexBuffer_;

	milk::graphics::Buffer indexBuffer_;

	size_t indexCount_;

	milk::graphics::PrimitiveTopology primitiveTopology_;

};

MAKE_POINTER_DEFINITIONS(DrawGroup);

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_DRAWGROUP_HPP_ */
