#ifndef _COCONUT_PULP_RENDERER_ORIENTEDCAMERA_HPP_
#define _COCONUT_PULP_RENDERER_ORIENTEDCAMERA_HPP_

#include "milk/utils/Lazy.hpp"

#include "Camera.hpp"
#include "Orientation.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class OrientedCamera : public Camera {
public:

	OrientedCamera() {
		orientation_.setScale(milk::math::Vector3d(1.0f, 1.0f, 1.0f));
	}

	const milk::math::Matrix& viewTransformation() const override {
		return orientation_.worldTransformation();
	}

	void setTranslation(const milk::math::Vector3d& translation) {
		orientation_.setTranslation(-translation);
	}

	milk::math::Vector3d getTranslation() const {
		return -orientation_.getTranslation();
	}

	void setRotation(const milk::math::Vector3d& rotation) {
		orientation_.setRotation(-rotation);
	}

	milk::math::Vector3d getRotation() const {
		return -orientation_.getRotation();
	}

private:

	Orientation orientation_;

};

MAKE_POINTER_DEFINITIONS(OrientedCamera);

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_ORIENTEDCAMERA_HPP_ */
