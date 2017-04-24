#if 0
#include "Rotation.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::pulp::math;

Rotation::Rotation(const Angle& angle, const Vec3& axis) :
	rotationQuaternion_(DirectX::XMQuaternionRotationAxis(axis.toXMVECTOR(), angle.radians()))
{
}

Rotation Rotation::interpolate(const Rotation& other, float factor) const {
	return Vec4(
		DirectX::XMQuaternionSlerp(
			rotationQuaternion_.toXMVECTOR(),
			other.rotationQuaternion_.toXMVECTOR(),
			factor
			)
		);
}

Vec3 Rotation::rotate(const Vec3& vector) const {
	return Vec3(DirectX::XMVector3Rotate(vector.toXMVECTOR(), rotationQuaternion_.toXMVECTOR()));
}
#endif 