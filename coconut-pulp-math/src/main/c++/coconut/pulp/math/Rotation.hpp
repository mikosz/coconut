#ifndef _COCONUT_MILK_MATH_ROTATION_HPP_
#define _COCONUT_MILK_MATH_ROTATION_HPP_

#include "Angle.hpp"
#include "Vector.hpp"

#include <DirectXMath.h>

namespace coconut {
namespace milk {
namespace math {

#pragma message("TODO: rewrite!")
class Rotation {
public:

	Rotation(const Angle& angle, const Vec3& axis);

	Rotation(const Vec4& quaternion) :
		rotationQuaternion_(quaternion)
	{
	}

	Rotation interpolate(const Rotation& other, float factor) const;

	Vec3 rotate(const Vec3& vector) const;

	const Vec4& rotationQuaternion() const {
		return rotationQuaternion_;
	}

private:

	Vec4 rotationQuaternion_;

};

inline Rotation interpolate(const Rotation& lhs, const Rotation& rhs, float factor) {
	return lhs.interpolate(rhs, factor);
}

inline Vec3 rotated(const Vec3& vector, const Rotation& rotation) {
	return rotation.rotate(vector);
}

inline void rotate(Vec3& vector, const Rotation& rotation) {
	vector = rotation.rotate(vector);
}

} // namespace math
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_MATH_ROTATION_HPP_ */
