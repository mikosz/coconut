#ifndef _COCONUT_PULP_RENDERER_ORIENTATION_HPP_
#define _COCONUT_PULP_RENDERER_ORIENTATION_HPP_

#include <functional>

#include "coconut/milk/math/Matrix.hpp"
#include "coconut/milk/math/Vector.hpp"
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

	const milk::math::Matrix& worldTransformation() const {
		return worldMatrix_.get();
	}

	void setTranslation(const milk::math::Vector3d& translation) {
		translation_ = translation;
		worldMatrix_.invalidate();
	}

	const milk::math::Vector3d& getTranslation() const {
		return translation_;
	}

	void translate(const milk::math::Vector3d& translation) {
		translation_ += translation;
		worldMatrix_.invalidate();
	}

	void setScale(const milk::math::Vector3d& scale) {
		scale_ = scale;
		worldMatrix_.invalidate();
	}

	const milk::math::Vector3d& getScale() const {
		return scale_;
	}

	void setRotation(const milk::math::Vector3d& rotation) {
		rotation_ = rotation;
		worldMatrix_.invalidate();
	}

	const milk::math::Vector3d& getRotation() const {
		return rotation_;
	}

	void rotate(const milk::math::Vector3d& rotation) {
		rotation_ += rotation;
		worldMatrix_.invalidate();
	}

private:

	milk::math::Vector3d translation_;

	milk::math::Vector3d scale_;

	milk::math::Vector3d rotation_;

	milk::utils::Lazy<milk::math::Matrix> worldMatrix_;

	void calculateWorldTransformation(milk::math::Matrix& matrix) {
		matrix =
			milk::math::Matrix::rotation(rotation_.x(), rotation_.y(), rotation_.z()) *
			milk::math::Matrix::scale(scale_.x(), scale_.y(), scale_.z()) *
			milk::math::Matrix::translation(translation_.x(), translation_.y(), translation_.z())
			;
	}

};

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_ORIENTATION_HPP_ */
