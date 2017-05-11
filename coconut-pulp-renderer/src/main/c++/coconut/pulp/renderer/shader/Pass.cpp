#include "Pass.hpp"

#include "coconut/pulp/renderer/PassContext.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;
using namespace coconut::pulp::renderer::shader;

void Pass::bind(DrawCommand& drawCommand, const Properties& properties) const {
	drawCommand.setInputLayout(&input_.layout());
	drawCommand.setVertexShader(&vertexShader_->shaderData());
	vertexShader_->bind(drawCommand, properties);

	if (geometryShader_) {
		drawCommand.setGeometryShader(&geometryShader_->shaderData());
		geometryShader_->bind(drawCommand, properties);
	}

	if (hullShader_) {
		drawCommand.setHullShader(&hullShader_->shaderData());
		hullShader_->bind(drawCommand, properties);
	}

	if (domainShader_) {
		drawCommand.setDomainShader(&domainShader_->shaderData());
		domainShader_->bind(drawCommand, properties);
	}

	drawCommand.setPixelShader(&pixelShader_->shaderData());
	pixelShader_->bind(drawCommand, properties);
}
