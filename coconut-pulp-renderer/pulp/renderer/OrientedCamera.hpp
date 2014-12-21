#ifndef _COCONUT_PULP_RENDERER_ORIENTEDCAMERA_HPP_
#define _COCONUT_PULP_RENDERER_ORIENTEDCAMERA_HPP_

#include "milk/utils/Lazy.hpp"

#include "Camera.hpp"
#include "Oriented.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class OrientedCamera : public Camera {
public:

	OrientedCamera() {
		oriented_.setScale(milk::math::Vector3d(1.0f, 1.0f, 1.0f));
	}

	const milk::math::Matrix& viewTransformation() const override {
		return oriented_.worldTransformation();
	}

	void setTranslation(const milk::math::Vector3d& translation) {
		oriented_.setTranslation(-translation);
	}

	milk::math::Vector3d getTranslation() const {
		return -oriented_.getTranslation();
	}

	void setRotation(const milk::math::Vector3d& rotation) {
		oriented_.setRotation(-rotation);
	}

	milk::math::Vector3d getRotation() const {
		return -oriented_.getRotation();
	}

private:

	Oriented oriented_;

};

MAKE_POINTER_DEFINITIONS(OrientedCamera);

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_ORIENTEDCAMERA_HPP_ */
