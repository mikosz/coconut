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
		transformation_(pulp::math::Matrix::IDENTITY),
		position_([this](pulp::math::Vec3& position) { position = transformation_.inverted().transposed().extractTranslation(); }) // TODO: could be done with fewer temporaries
	{
	}

	const pulp::math::Matrix& viewTransformation() const override {
		return transformation_;
	}

	const pulp::math::Vec3& position() const override {
		return position_.get();
	}

	void reset() {
		transformation_ = pulp::math::Matrix::IDENTITY;
		position_.invalidate();
	}

	void translate(const pulp::math::Vec3& translation) {
		transformation_ *= pulp::math::Matrix::translation(-translation);
		position_.invalidate();
	}

	void rotate(const pulp::math::Vec3& rotation) {
		transformation_ *= pulp::math::Matrix::rotation(-rotation);
		position_.invalidate();
	}

private:

	pulp::math::Matrix transformation_;

	milk::utils::Lazy<pulp::math::Vec3> position_;

};

CCN_MAKE_POINTER_DEFINITIONS(OrientedCamera);

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_ORIENTEDCAMERA_HPP_ */
