#ifndef _COCONUT_PULP_RENDERER_ORIENTEDCAMERA_HPP_
#define _COCONUT_PULP_RENDERER_ORIENTEDCAMERA_HPP_

#include "coconut/pulp/math/Matrix.hpp"
#include "coconut/pulp/math/Vector.hpp"

#include "coconut/milk/utils/Lazy.hpp"

#include "Camera.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class OrientedCamera : public Camera {
public:

	OrientedCamera() :
		transformation_(milk::math::Matrix::IDENTITY),
		position_([this](milk::math::Vector3d& position) { position = transformation_.inverted().transposed().extractTranslation(); }) // TODO: could be done with fewer temporaries
	{
	}

	const milk::math::Matrix& viewTransformation() const override {
		return transformation_;
	}

	const milk::math::Vector3d& position() const override {
		return position_.get();
	}

	void reset() {
		transformation_ = milk::math::Matrix::IDENTITY;
		position_.invalidate();
	}

	void translate(const milk::math::Vector3d& translation) {
		transformation_ *= milk::math::Matrix::translation(-translation);
		position_.invalidate();
	}

	void rotate(const milk::math::Vector3d& rotation) {
		transformation_ *= milk::math::Matrix::rotation(-rotation);
		position_.invalidate();
	}

private:

	milk::math::Matrix transformation_;

	milk::utils::Lazy<milk::math::Vector3d> position_;

};

CCN_MAKE_POINTER_DEFINITIONS(OrientedCamera);

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_ORIENTEDCAMERA_HPP_ */
