#include "Pass.hpp"

#include "coconut/pulp/renderer/PassContext.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;
using namespace coconut::pulp::renderer::shader;

void Pass::bind(DrawCommand& drawCommand, const PassContext& passContext) const {
	vertexShader_->bind(drawCommand, passContext);
	if (geometryShader_) {
		geometryShader_->bind(drawCommand, passContext);
	}
	if (hullShader_) {
		hullShader_->bind(drawCommand, passContext);
	}
	if (domainShader_) {
		domainShader_->bind(drawCommand, passContext);
	}
	pixelShader_->bind(drawCommand, passContext);
}
