#include "GeometryDrawCommand.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;

DrawCommand::Key GeometryDrawCommand::key() const {

}

void GeometryDrawCommand::run(milk::graphics::Device& graphicsDevice) {
	graphicsDevice.setVertexShader(*vertexShader_);
	graphicsDevice.setPixelShader(*pixelShader_);
}
