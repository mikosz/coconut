#include "Pass.hpp"

#include "coconut/pulp/renderer/PassContext.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;
using namespace coconut::pulp::renderer::shader;

void Pass::bind(DrawCommand& drawCommand, const PassContext& passContext) const {
	drawCommand.setInputLayout(&input_->layout());
	drawCommand.setVertexShader(&vertexShader_->shaderData());
	vertexShader_->bind(drawCommand, passContext);

	if (geometryShader_) {
		drawCommand.setGeometryShader(&geometryShader_->shaderData());
		geometryShader_->bind(drawCommand, passContext);
	}

	if (hullShader_) {
		hullShader_->bind(drawCommand, passContext);
	}

	if (domainShader_) {
		domainShader_->bind(drawCommand, passContext);
	}

	drawCommand.setPixelShader(&pixelShader_->shaderData());
	pixelShader_->bind(drawCommand, passContext);
}
