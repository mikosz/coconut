#include "GeometryDrawCommand.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;

DrawCommand::Key GeometryDrawCommand::key() const {

}

void GeometryDrawCommand::run(milk::graphics::CommandList& commandList) {
	DrawCommand::run(commandList);

	assert(vertexBuffer_ != nullptr);
	assert(indexBuffer_ != nullptr);
	assert(renderingContext_ != nullptr);
	assert(indexCount_ > 0);
	assert(primitiveTopology_ != milk::graphics::PrimitiveTopology::INVALID);

	commandList.setVertexBuffer(*vertexBuffer_, 0);
	commandList.setIndexBuffer(*indexBuffer_, 0);
	
	commandList.draw(0, indexCount_, primitiveTopology_); 
}
