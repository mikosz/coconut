#ifndef _COCONUT_PULP_RENDERER_DRAWGROUP_HPP_
#define _COCONUT_PULP_RENDERER_DRAWGROUP_HPP_

#include <vector>

#include "coconut/milk/graphics/IndexBuffer.hpp"
#include "coconut/milk/graphics/VertexBuffer.hpp"
#include "coconut/milk/graphics/PrimitiveTopology.hpp"
#include "coconut/milk/graphics/VertexInterface.hpp"
#include "coconut/milk/graphics/InputLayoutDescription.hpp"
#include "coconut/milk/graphics/Rasteriser.hpp"
#include "coconut/milk/graphics/Renderer.hpp"

#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

#include "coconut/pulp/model/Data.hpp"

#include "shader/Shader.hpp"
#include "PassContext.hpp"
#include "Material.hpp"
#include "MaterialManager.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class CommandBuffer;

class DrawGroup {
public:

	DrawGroup(
		const model::Data& modelData,
		size_t groupIndex,
		milk::graphics::Renderer& graphicsRenderer,
		const milk::graphics::InputLayoutDescription& inputLayoutDescription,
		const MaterialManager& materialManager
		);

	void render(CommandBuffer& commandBuffer, PassContext passContext);

private:

	using Samplers = std::vector<std::string>;

	MaterialSharedPtr material_;

	milk::graphics::Rasteriser rasteriser_;

	Samplers samplerIds_;

	milk::graphics::VertexBuffer vertexBuffer_;

	milk::graphics::VertexBuffer instanceDataBuffer_;

	milk::graphics::IndexBuffer indexBuffer_;

	size_t indexCount_;

	milk::graphics::PrimitiveTopology primitiveTopology_;

};

CCN_MAKE_POINTER_DEFINITIONS(DrawGroup);

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_DRAWGROUP_HPP_ */
