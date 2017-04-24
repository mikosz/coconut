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

	const Matrix4x4& worldTransformation() const {
		return worldMatrix_.get();
	}

	void setTranslation(const Vec3& translation) {
		translation_ = translation;
		worldMatrix_.invalidate();
	}

	const Vec3& getTranslation() const {
		return translation_;
	}

	void translate(const Vec3& translation) {
		translation_ += translation;
		worldMatrix_.invalidate();
	}

	void setScale(const Vec3& scale) {
		scale_ = scale;
		worldMatrix_.invalidate();
	}

	const Vec3& getScale() const {
		return scale_;
	}

	void setRotation(const Vec3& rotation) {
		rotation_ = rotation;
		worldMatrix_.invalidate();
	}

	const Vec3& getRotation() const {
		return rotation_;
	}

	void rotate(const Vec3& rotation) {
		rotation_ += rotation;
		worldMatrix_.invalidate();
	}

private:

	Vec3 translation_;

	Vec3 scale_;

	Vec3 rotation_;

	milk::utils::Lazy<Matrix4x4> worldMatrix_;

	void calculateWorldTransformation(Matrix4x4& matrix) {
		matrix = Matrix4x4::IDENTITY;
		matrix =
			Matrix4x4::rotation(Vec3(1.0f, 0.0f, 0.0f), radians(rotation_.x())) * // Vec3 constants for unit vectors
			Matrix4x4::rotation(Vec3(0.0f, 1.0f, 0.0f), radians(rotation_.y())) *
			Matrix4x4::rotation(Vec3(0.0f, 0.0f, 1.0f), radians(rotation_.z())) *
			Matrix4x4::scale(scale_) *
			Matrix4x4::translation(translation_)
			;
	}

};

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_ORIENTATION_HPP_ */
