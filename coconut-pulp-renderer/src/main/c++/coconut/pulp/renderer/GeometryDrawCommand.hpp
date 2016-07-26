#ifndef _COCONUT_PULP_RENDERER_GEOMETRY_DRAW_COMMAND_HPP_
#define _COCONUT_PULP_RENDERER_GEOMETRY_DRAW_COMMAND_HPP_

#include "coconut/milk/graphics/CommandList.hpp"
#include "coconut/milk/graphics/Shader.hpp"
#include "coconut/milk/graphics/VertexBuffer.hpp"
#include "coconut/milk/graphics/IndexBuffer.hpp"
#include "coconut/milk/graphics/PrimitiveTopology.hpp"

#include "DrawCommand.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

// TODO: there's no sense for a "GeometryDraw" command. Should be a "IndexedDrawCommand" etc
class GeometryDrawCommand : public DrawCommand {
public:

	Key key() const override;

	void submit(milk::graphics::CommandList& commandList) override;

	// TODO: pointers!
	void setVertexBuffer(milk::graphics::VertexBuffer* vertexBuffer) {
		vertexBuffer_ = vertexBuffer;
	}

	void setIndexBuffer(milk::graphics::IndexBuffer* indexBuffer) {
		indexBuffer_ = indexBuffer;
	}

	void setIndexCount(size_t indexCount) {
		indexCount_ = indexCount;
	}

	void setPrimitiveTopology(milk::graphics::PrimitiveTopology primitiveTopology) {
		primitiveTopology_ = primitiveTopology;
	}

private:

	milk::graphics::VertexBuffer* vertexBuffer_ = nullptr;

	milk::graphics::IndexBuffer* indexBuffer_ = nullptr;

	size_t indexCount_ = 0;

	milk::graphics::PrimitiveTopology primitiveTopology_ = milk::graphics::PrimitiveTopology::INVALID;

};

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_GEOMETRY_DRAW_COMMAND_HPP_ */
