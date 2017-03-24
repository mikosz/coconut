#ifndef _COCONUT_PULP_RENDERER_ORIENTATION_HPP_
#define _COCONUT_PULP_RENDERER_ORIENTATION_HPP_

#include <functional>

#include "coconut/pulp/math/Matrix.hpp"
#include "coconut/pulp/math/Vector.hpp"
#include "coconut/milk/utils/Lazy.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

// TODO: this is useless, I think. It won't allow to stack transformations, just holds a single translation, scale and rotation.
class Orientation {
public:

	Orientation() :
		translation_(0.0f, 0.0f, 0.0f),
		scale_(1.0f, 1.0f, 1.0f),
		rotation_(0.0f, 0.0f, 0.0f),
		worldMatrix_(std::bind(&Orientation::calculateWorldTransformation, this, std::placeholders::_1))
	{
	}

	const pulp::math::Matrix& worldTransformation() const {
		return worldMatrix_.get();
	}

	void setTranslation(const pulp::math::Vec3& translation) {
		translation_ = translation;
		worldMatrix_.invalidate();
	}

	const pulp::math::Vec3& getTranslation() const {
		return translation_;
	}

	void translate(const pulp::math::Vec3& translation) {
		translation_ += translation;
		worldMatrix_.invalidate();
	}

	void setScale(const pulp::math::Vec3& scale) {
		scale_ = scale;
		worldMatrix_.invalidate();
	}

	const pulp::math::Vec3& getScale() const {
		return scale_;
	}

	void setRotation(const pulp::math::Vec3& rotation) {
		rotation_ = rotation;
		worldMatrix_.invalidate();
	}

	const pulp::math::Vec3& getRotation() const {
		return rotation_;
	}

	void rotate(const pulp::math::Vec3& rotation) {
		rotation_ += rotation;
		worldMatrix_.invalidate();
	}

private:

	pulp::math::Vec3 translation_;

	pulp::math::Vec3 scale_;

	pulp::math::Vec3 rotation_;

	milk::utils::Lazy<pulp::math::Matrix> worldMatrix_;

	void calculateWorldTransformation(pulp::math::Matrix& matrix) {
		matrix =
			pulp::math::Matrix::rotation(rotation_.x(), rotation_.y(), rotation_.z()) *
			pulp::math::Matrix::scale(scale_.x(), scale_.y(), scale_.z()) *
			pulp::math::Matrix::translation(translation_.x(), translation_.y(), translation_.z())
			;
	}

};

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_ORIENTATION_HPP_ */
