#include "Actor.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;

void Actor::render(milk::graphics::Device& graphicsDevice, RenderingContext renderingContext) {
	renderingContext.actor = this;

	model_->render(graphicsDevice, renderingContext);
}
