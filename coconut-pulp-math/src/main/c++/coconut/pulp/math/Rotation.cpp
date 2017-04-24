#include "Rotation.hpp"

#include <cmath>

#include <coconut-tools/exceptions/LogicError.hpp>

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::math;

Rotation::Rotation(const Angle& angle, const Vec3& axis) :
	q_(std::cos(angle.radians() / 2.0f), std::sin(angle.radians() / 2.0f) * axis)
{
	if (!ScalarEqual<float>()(axis.lengthSq(), 1.0f)) {
		throw coconut_tools::exceptions::LogicError("Expected unit-length axis");
	}
}

Rotation Rotation::interpolate(const Rotation& other, float factor) const {
	
}

Vec3 Rotation::rotate(const Vec3& vector) const {
	
}
