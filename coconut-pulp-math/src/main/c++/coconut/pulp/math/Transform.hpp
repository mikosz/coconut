#ifndef _COCONUT_PULP_MATH_TRANSFORM_HPP_
#define _COCONUT_PULP_MATH_TRANSFORM_HPP_

#include "Matrix.hpp"
#include "Orientation.hpp"

namespace coconut {
namespace pulp {
namespace math {

class Transform {
public:

	/**
	 * Creates a perspective projection transformation. Positions within the view frustum are mapped into
	 * the homogeneous clip-space <[-1, 1], [-1, 1], [0, 1], 1> (DirectX style).
	 */
	static Transform orthographicProjection(
		Handedness handedness,
		float left,
		float right,
		float top,
		float bottom,
		float near,
		float far
		) noexcept;

	/**
	 * Creates a perspective projection transformation. Positions within the view frustum are mapped into
	 * the homogeneous clip-space <[-1, 1], [-1, 1], [0, 1], 1> * w (DirectX style).
	 */
	static Transform perspectiveProjection(
		Handedness handedness,
		Angle verticalFOV,
		float aspectRatio,
		float near,
		float far
		) noexcept;

	static Transform translation(const Vec3& vector) noexcept;

	static Transform scale(const Vec3& by) noexcept;

	static Transform rotation(Vec3 around, Angle by) noexcept;

	Transform() noexcept = default;

	Transform(Matrix4x4 transformationMatrix) noexcept :
		matrix_(std::move(transformationMatrix))
	{
	}

	Transform(const Orientation& orientation) noexcept;

	// TODO: what about Vec3? Need to make a Point and Vector(?) class?
	Vec4 apply(const Vec4& vector) const noexcept {
		// TODO: this is ineffective. Need matrix data to be stored in columns rather than rows, OR better yet,
		// multiply by rows, like everyone in maths does (as does OpenGL).
		return Vec4(
			dot(matrix_.column(0), vector),
			dot(matrix_.column(1), vector),
			dot(matrix_.column(2), vector),
			dot(matrix_.column(3), vector)
			);
	}

	Transform& append(const Transform& next) noexcept {
		matrix_ *= next.matrix_;
		return *this;
	}

	Transform then(const Transform& next) const noexcept {
		return Transform(*this).append(next.matrix_);
	}

	const Matrix4x4& matrix() const {
		return matrix_;
	}

private:

	Matrix4x4 matrix_ = Matrix4x4::IDENTITY;

};

inline Transform operator<<(const Transform& first, const Transform& second) {
	return first.then(second);
}

} // namespace math

using math::Transform;

} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_MATH_TRANSFORM_HPP_ */
