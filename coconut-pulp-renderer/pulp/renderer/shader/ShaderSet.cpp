#include "ShaderSet.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;
using namespace coconut::pulp::renderer::shader;

void ShaderSet::bind(milk::graphics::Device& graphicsDevice, RenderingContext renderingContext) {
	vertexShader_->bind(graphicsDevice, renderingContext);
	pixelShader_->bind(graphicsDevice, renderingContext);
}
