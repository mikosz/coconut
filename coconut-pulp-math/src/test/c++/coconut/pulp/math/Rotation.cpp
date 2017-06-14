#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include "coconut/pulp/math/Angle.hpp"
#include "coconut/pulp/math/Rotation.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::math;

namespace /* anonymous */ {

BOOST_AUTO_TEST_SUITE(PulpTestSuite);
BOOST_AUTO_TEST_SUITE(PulpMathTestSuite);
BOOST_AUTO_TEST_SUITE(PulpMathRotationTestSuite);

BOOST_AUTO_TEST_CASE(AppliesRotationToVector) {
	const auto rotation = Rotation({ 2.0f, 1.0f, 3.0f }, -23.0_deg);

	const auto start = Vec3(1.0f, 3.0f, 5.0f);
	const auto end = Vec3(1.565343f, 3.60607f, 4.421081f);
	const auto rotated = rotation.apply(start);
	BOOST_CHECK_EQUAL(rotated, end);
}

BOOST_AUTO_TEST_CASE(AppendsRotations) {
	const auto first = Rotation({ 2.0f, 1.0f, 3.0f }, -23.0_deg);
	const auto then = Rotation({ 0.0f, 1.0f, 0.0f }, 180.0_deg);
	const auto rotation = first << then;

	const auto start = Vec3(1.0f, 3.0f, 5.0f);
	const auto end = Vec3(-1.565343f, 3.60607f, -4.421081f);
	const auto rotated = rotation.apply(start);
	BOOST_CHECK_EQUAL(rotated, end);
}

BOOST_AUTO_TEST_CASE(LinearlyInterpolatesRotations) {
	const auto r1 = Rotation({ 0.0f, 1.0f, 0.0f }, 0.0_deg);
	const auto r2 = Rotation({ 0.0f, 1.0f, 0.0f }, 10.0_deg);

	const auto interpolated = lerp(r1, r2, 0.25f);
	const auto expected = Rotation({ 0.0f, 1.0f, 0.0f }, 2.5_deg);

	BOOST_CHECK_EQUAL(interpolated.rotationQuaternion(), expected.rotationQuaternion());
}

BOOST_AUTO_TEST_CASE(SphericallyLinearlyInterpolatesRotations) {
	const auto r1 = Rotation({ 0.0f, 1.0f, 0.0f }, 0.0_deg);
	const auto r2 = Rotation({ 0.0f, 1.0f, 0.0f }, 90.0_deg);

	const auto interpolated = slerp(r1, r2, 0.25f);
	const auto expected = Rotation({ 0.0f, 1.0f, 0.0f }, degrees(90.0f * 0.25f));

	BOOST_CHECK_EQUAL(interpolated.rotationQuaternion(), expected.rotationQuaternion());
}

BOOST_AUTO_TEST_SUITE_END(/* PulpMathRotationTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* PulpMathTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* PulpTestSuite */);

} // anonymous namespace
