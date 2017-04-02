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
		transformation_(Matrix4x4::IDENTITY),
		position_([this](Vec3& position) {
				position = transformation_.inverse().transpose()[3].xyz();
			}) // TODO: could be done with fewer temporaries
	{
	}

	const Matrix4x4& viewTransformation() const override {
		return transformation_;
	}

	const Vec3& position() const override {
		return position_.get();
	}

	void reset() {
		transformation_ = Matrix4x4::IDENTITY;
		position_.invalidate();
	}

	void translate(const Vec3& translation) {
		transformation_ *= Matrix4x4::translation(-translation);
		position_.invalidate();
	}

	void rotate(const Vec3& rotation) {
#pragma message("TODO: unimplemented")
		//transformation_ *= Matrix4x4::rotation(-rotation);
		position_.invalidate();
	}

private:

	Matrix4x4 transformation_;

	milk::utils::Lazy<Vec3> position_;

};

CCN_MAKE_POINTER_DEFINITIONS(OrientedCamera);

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_ORIENTEDCAMERA_HPP_ */
