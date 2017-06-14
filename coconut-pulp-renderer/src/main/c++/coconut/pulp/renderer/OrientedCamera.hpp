#ifndef _COCONUT_PULP_RENDERER_ORIENTEDCAMERA_HPP_
#define _COCONUT_PULP_RENDERER_ORIENTEDCAMERA_HPP_

#include "coconut/pulp/math/Transform.hpp"
#include "coconut/pulp/math/Vector.hpp"

#include "coconut/milk/utils/Lazy.hpp"

#include "Camera.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

// TODO: humbug
class OrientedCamera : public Camera {
public:

	OrientedCamera() :
		position_([this](Vec3& position) {
				position = transform_.matrix().transpose().inverse()[3].xyz();
			}) // TODO: check if inverse / transposed needed. Use view.
	{
	}

	const Matrix4x4& viewTransform() const override {
		return transform_.matrix();
	}

	const Vec3& position() const override {
		return position_.get();
	}

	void reset() {
		transform_ = Transform();
		position_.invalidate();
	}

	void translate(const Vec3& translation) {
		transform_.append(Transform::translation(-translation));
		position_.invalidate();
	}

	void rotate(const Vec3& around, Angle by) {
		transform_.append(Transform::rotation(around, -by));
		position_.invalidate();
	}

private:

	Transform transform_;

	milk::utils::Lazy<Vec3> position_;

};

CT_MAKE_POINTER_DEFINITIONS(OrientedCamera);

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_ORIENTEDCAMERA_HPP_ */
