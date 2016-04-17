#include "Actor.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;

void Actor::render(CommandBuffer& commandBuffer, RenderingContext renderingContext) {
	renderingContext.actor = this;

	model_->render(commandBuffer, renderingContext);
}
