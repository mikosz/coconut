#include "Angle.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::math;

const Angle Angle::RIGHT(::radians(PI / 2.0f));
const Angle Angle::HALF_FULL(PI);
const Angle Angle::FULL(::radians(2.0f * PI));

std::ostream& coconut::pulp::math::operator<<(std::ostream& os, const Angle& angle) {
	return os << angle.radians();
}
