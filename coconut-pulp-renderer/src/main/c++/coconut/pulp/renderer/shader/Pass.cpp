#include "Pass.hpp"

#include "../RenderingContext.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;
using namespace coconut::pulp::renderer::shader;

std::unique_ptr<DrawCommand> Pass::createDrawCommand(const RenderingContext& renderingContext) const {
	vertexShader_->update
}
