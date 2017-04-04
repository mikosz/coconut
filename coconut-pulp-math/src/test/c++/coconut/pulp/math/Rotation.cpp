#if 0
#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include "coconut/pulp/math/Rotation.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::pulp::math;

namespace /* anonymous */ {

BOOST_AUTO_TEST_SUITE(MilkMathRotationTestSuite);

BOOST_AUTO_TEST_CASE(InterpolatedRotationFactor0IsFirstRotation) {
	Rotation rotation1(radians(1.0f), Vec3(1.0f, 2.0f, 3.0f));
	Rotation rotation2(radians(2.0f), Vec3(4.0f, 5.0f, 6.0f));

	Rotation interpolated = interpolate(rotation1, rotation2, 0.0f);

	BOOST_CHECK(interpolated.rotationQuaternion().almostEqual(rotation1.rotationQuaternion(), 0.00001f));
}

BOOST_AUTO_TEST_CASE(InterpolatedRotationFactor1IsSecondRotation) {
	Rotation rotation1(radians(1.0f), Vec3(1.0f, 2.0f, 3.0f));
	Rotation rotation2(radians(2.0f), Vec3(4.0f, 5.0f, 6.0f));

	Rotation interpolated = interpolate(rotation1, rotation2, 1.0f);

	BOOST_CHECK(interpolated.rotationQuaternion().almostEqual(rotation2.rotationQuaternion(), 0.00001f));
}

BOOST_AUTO_TEST_SUITE_END(/* MilkMathRotationTestSuite */);

} // anonymous namespace
#endif
