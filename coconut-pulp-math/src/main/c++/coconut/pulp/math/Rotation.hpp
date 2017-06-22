#ifndef _COCONUT_MILK_MATH_ROTATION_HPP_
#define _COCONUT_MILK_MATH_ROTATION_HPP_

#include "Quaternion.hpp"
#include "Vector.hpp"

namespace coconut {
namespace pulp {
namespace math {

class Angle;

class Rotation {
public:

	Rotation(const Vec3& axis, const Angle& angle);

	Rotation(const Quat& quaternion) :
		rotationQuaternion_(quaternion)
	{
	}

	Vec3 apply(const Vec3& vector) const;

	Rotation lerp(const Rotation& other, float factor) const;

	Rotation slerp(const Rotation& other, float factor) const;

	Rotation& append(const Rotation& next) noexcept {
		rotationQuaternion_ = next.rotationQuaternion_ * rotationQuaternion_;
		return *this;
	}

	Rotation then(const Rotation& next) const noexcept {
		return Rotation(*this).append(next);
	}

	const Quat& rotationQuaternion() const {
		return rotationQuaternion_;
	}

private:

	Quat rotationQuaternion_;

};

inline Rotation operator<<(const Rotation& first, const Rotation& second) {
	return first.then(second);
}

inline Rotation lerp(const Rotation& lhs, const Rotation& rhs, float factor) {
	return lhs.lerp(rhs, factor);
}

inline Rotation slerp(const Rotation& lhs, const Rotation& rhs, float factor) {
	return lhs.slerp(rhs, factor);
}

inline Vec3 rotated(const Vec3& vector, const Rotation& rotation) {
	return rotation.apply(vector);
}

inline void rotate(Vec3& vector, const Rotation& rotation) {
	vector = rotation.apply(vector);
}

} // namespace math
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_MILK_MATH_ROTATION_HPP_ */
