#ifndef _COCONUT_MILK_MATH_ROTATION_HPP_
#define _COCONUT_MILK_MATH_ROTATION_HPP_

#include "Angle.hpp"
#include "Vector.hpp"

#include <DirectXMath.h>

namespace coconut {
namespace milk {
namespace math {

class Rotation {
public:

	Rotation(const Angle& angle, const Vector3d& axis);

	Rotation(const Vector4d& quaternion) :
		rotationQuaternion_(quaternion)
	{
	}

	Rotation interpolate(const Rotation& other, float factor) const;

	Vector3d rotate(const Vector3d& vector) const;

	const Vector4d& rotationQuaternion() const {
		return rotationQuaternion_;
	}

private:

	Vector4d rotationQuaternion_;

};

inline Rotation interpolate(const Rotation& lhs, const Rotation& rhs, float factor) {
	return lhs.interpolate(rhs, factor);
}

inline Vector3d rotated(const Vector3d& vector, const Rotation& rotation) {
	return rotation.rotate(vector);
}

inline void rotate(Vector3d& vector, const Rotation& rotation) {
	vector = rotation.rotate(vector);
}

} // namespace math
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_MATH_ROTATION_HPP_ */
