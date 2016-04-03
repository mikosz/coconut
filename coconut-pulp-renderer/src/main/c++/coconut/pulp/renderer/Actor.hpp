#ifndef _COCONUT_PULP_RENDERER_ACTOR_HPP_
#define _COCONUT_PULP_RENDERER_ACTOR_HPP_

#include <vector>

#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"
#include "coconut/milk/graphics/Device.hpp"

#include "Model.hpp"
#include "Orientation.hpp"
#include "RenderingContext.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class Actor {
public:

	Actor(ModelSharedPtr model) :
		model_(model)
	{
	}

	void render(milk::graphics::Device& graphicsDevice, RenderingContext renderingContext);

	// TODO: move below functions to interface?
	const milk::math::Matrix& worldTransformation() const {
		return orientation_.worldTransformation();
	}

	void setTranslation(const milk::math::Vector3d& translation) {
		orientation_.setTranslation(translation);
	}

	const milk::math::Vector3d& getTranslation() const {
		return orientation_.getTranslation();
	}

	void setScale(const milk::math::Vector3d& scale) {
		orientation_.setScale(scale); // TODO: block non-uniform scale?
	}

	const milk::math::Vector3d& getScale() const {
		return orientation_.getScale();
	}

	void setRotation(const milk::math::Vector3d& rotation) {
		orientation_.setRotation(rotation);
	}

	const milk::math::Vector3d& getRotation() const {
		return orientation_.getRotation();
	}

private:

	ModelSharedPtr model_;

	Orientation orientation_;

};

CCN_MAKE_POINTER_DEFINITIONS(Actor);

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_ACTOR_HPP_ */
