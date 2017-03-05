#ifndef _COCONUT_PULP_RENDERER_DRAWGROUP_HPP_
#define _COCONUT_PULP_RENDERER_DRAWGROUP_HPP_

#include <vector>

#include <boost/optional.hpp>

#include "coconut/milk/graphics/Renderer.hpp"
#include "coconut/milk/graphics/IndexBuffer.hpp"
#include "coconut/milk/graphics/VertexBuffer.hpp"
#include "coconut/milk/graphics/PrimitiveTopology.hpp"

#include "coconut/pulp/renderer/CommandBuffer.hpp"
#include "coconut/pulp/renderer/shader/Input.hpp"
#include "coconut/pulp/mesh/Submesh.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class CommandBuffer;

class DrawGroup {
public:

	DrawGroup(
		milk::graphics::Renderer& graphicsRenderer,
		const shader::Input& shaderInput,
		const mesh::Submesh& submesh
	);

	void render(CommandBuffer& commandBuffer, PassContext passContext);

private:

	milk::graphics::VertexBuffer vertexBuffer_;

	boost::optional<milk::graphics::VertexBuffer> instanceDataBuffer_;

	size_t instanceCount_;

	boost::optional<milk::graphics::IndexBuffer> indexBuffer_;

	size_t indexCount_;

	milk::graphics::PrimitiveTopology primitiveTopology_;

};

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_DRAWGROUP_HPP_ */
