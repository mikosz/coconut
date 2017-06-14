#include "Rotation.hpp"

#include "Angle.hpp"
#include "Vector.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::math;

namespace /* anonymous */
{

Quat createRotationQuaternion(const Angle& angle, const Vec3& axis) {
	return Quat(std::cos(angle.radians() / 2.0f), std::sin(angle.radians() / 2.0f) * axis.normalised());
}

} // anonymous namespace

Rotation::Rotation(const Vec3& axis, const Angle& angle) :
	rotationQuaternion_(createRotationQuaternion(angle, axis))
{
}

Rotation Rotation::lerp(const Rotation& other, float factor) const {
	auto q = (1.0f - factor) * rotationQuaternion_ + factor * other.rotationQuaternion_;
	q.normalise();
	return Rotation(std::move(q));
}

Rotation Rotation::slerp(const Rotation& other, float factor) const {
	const auto lhs = rotationQuaternion_;
	auto rhs = other.rotationQuaternion_;
	auto qDot = dot(lhs, rhs);

	if (qDot < 0.0f) {
		rhs = -1.0f * rhs;
		qDot = -qDot;
	}

	const auto theta = std::acos(qDot);
	const auto sinTheta1minF = std::sin(theta * (1.0f - factor));
	const auto sinThetaF = std::sin(theta * factor);
	const auto sinTheta = std::sin(theta);

	if (ScalarEqual<float>()(sinTheta, 0.0f)) {
		return *this;
	}

	const auto lhsFactor = lhs * (sinTheta1minF / sinTheta);
	const auto rhsFactor = rhs * (sinThetaF / sinTheta);

	return Rotation(lhsFactor + rhsFactor);
}

Vec3 Rotation::apply(const Vec3& vector) const {
	return (rotationQuaternion_ * Quat(0.0f, vector) * rotationQuaternion_.inverse()).v();
}
