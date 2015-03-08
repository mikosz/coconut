#include "Rotation.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::math;

Rotation::Rotation(const Angle& angle, const Vector3d& axis) :
	rotationQuaternion_(DirectX::XMQuaternionRotationAxis(axis.toXMVECTOR(), angle.radians()))
{
}

Rotation Rotation::interpolate(const Rotation& other, float factor) const {
	return Vector4d(
		DirectX::XMQuaternionSlerp(
			rotationQuaternion_.toXMVECTOR(),
			other.rotationQuaternion_.toXMVECTOR(),
			factor
			)
		);
}

Vector3d Rotation::rotate(const Vector3d& vector) const {
	return Vector3d(DirectX::XMVector3Rotate(vector.toXMVECTOR(), rotationQuaternion_.toXMVECTOR()));
}
