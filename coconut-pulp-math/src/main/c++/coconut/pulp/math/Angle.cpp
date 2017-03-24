#if 0
#include "Angle.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::pulp::math;

namespace /* anonymous */ {

const float DEGREES_TO_RADIANS_MODIFIER = PI / 180.0f;
const float RADIANS_TO_DEGREES_MODIFIER = 180.0f / PI;

} // anonymous namespace

const Angle Angle::RIGHT(::radians(PI / 2.0f));
const Angle Angle::HALF_FULL(PI);
const Angle Angle::FULL(::radians(2.0f * PI));

float Angle::degrees() const {
	return radians_ * RADIANS_TO_DEGREES_MODIFIER;
}

std::ostream& coconut::pulp::math::operator<<(std::ostream& os, const Angle& angle) {
	return os << angle.radians();
}

Angle coconut::pulp::math::radians(float radians) {
	return Angle(radians);
}

Angle coconut::pulp::math::degrees(float degrees) {
	return Angle(degrees * DEGREES_TO_RADIANS_MODIFIER);
}
#endif