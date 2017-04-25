#ifndef _COCONUT_PULP_MATH_ROTATION_HPP_
#define _COCONUT_PULP_MATH_ROTATION_HPP_

#include <cmath>

#include "Angle.hpp"
#include "Vector.hpp"
#include "Quaternion.hpp"
#include "ScalarEqual.hpp"

namespace coconut {
namespace pulp {
namespace math {

class Rotation {
public:

	Rotation::Rotation(const Angle& angle, Vec3 axis) noexcept :
		q_(std::cos(angle.radians() / 2.0f), std::sin(angle.radians() / 2.0f) * axis.normalise())
	{
	}

	Rotation(Quat q) noexcept :
		q_(std::move(q))
	{
	}

	Rotation lerp(const Rotation& other, float factor) const noexcept {
		return ((1.0f - factor) * q_ + factor * other.q_).normalise();
	}

	Rotation slerp(const Rotation& other, float factor) const noexcept {
		const auto theta = std::acos(q_.s() * other.q_.s() + dot(q_.v(), other.q_.v()));
		const auto thisRatio = std::sin(theta * (1 - factor));
		const auto otherRatio = std::sin(theta * factor);

		return (thisRatio * q_ + otherRatio * other.q_) / std::sin(theta);
	}

	Vec3 rotate(const Vec3& vector) const noexcept {
		return (q_ * Quat(0.0f, vector) * q_.inverse()).v();
	}

	const Quat& quaternion() const noexcept {
		return q_;
	}

private:

	Quat q_;

};

inline Rotation lerp(const Rotation& lhs, const Rotation& rhs, float factor) noexcept {
	return lhs.lerp(rhs, factor);
}

inline Rotation slerp(const Rotation& lhs, const Rotation& rhs, float factor) noexcept {
	return lhs.slerp(rhs, factor);
}

} // namespace math
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_MATH_ROTATION_HPP_ */
