#include "Vector.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::math;

// TODO: put this in the .hpp file somehow
const Vector<1> Vector<1>::ZERO(0.0f, 0.0f, 0.0f, 0.0f);
const Vector<2> Vector<2>::ZERO(0.0f, 0.0f, 0.0f, 0.0f);
const Vector<3> Vector<3>::ZERO(0.0f, 0.0f, 0.0f, 0.0f);
const Vector<4> Vector<4>::ZERO(0.0f, 0.0f, 0.0f, 0.0f);

Vector3d coconut::milk::math::cross(const Vector3d& lhs, const Vector3d& rhs) {
	return lhs.cross(rhs);
}
