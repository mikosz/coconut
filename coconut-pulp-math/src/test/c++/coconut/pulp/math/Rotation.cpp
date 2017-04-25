#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include "coconut/pulp/math/Rotation.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::math;

namespace /* anonymous */ {

BOOST_AUTO_TEST_SUITE(PulpTestSuite);
BOOST_AUTO_TEST_SUITE(PulpMathTestSuite);
BOOST_AUTO_TEST_SUITE(PulpMathRotationTestSuite);

BOOST_AUTO_TEST_CASE(VectorsAreRotatedAroundOrigin) {
	const auto pre = Vec3(0.34f, 2.4f, 1.14f);
	const auto axis = Vec3(0.8f, 0.4f, 0.2f).normalised();
	const auto angle = degrees(21.0f);
	const auto expected = Vec3(0.400388f, 1.956757f, 1.784935f);

	BOOST_CHECK_EQUAL(Rotation(angle, axis).rotate(pre), expected);
}

BOOST_AUTO_TEST_CASE(RotationsCanBeInterpolated) {
	const auto r1 = Rotation(degrees(10.0f), { 1.2f, 1.0f, 3.1f });
	const auto r2 = Rotation(degrees(120.0f), { 0.2f, 1.0f, 0.3f });
	const auto interpolatedHalf = Rotation(Quat(0.851849f, { 0.109921f, 0.478132f, 0.183465f }));
	const auto interpolatedTenth = Rotation(Quat(0.986452f, { 0.047355f, 0.119935f, 0.101411f }));

	BOOST_CHECK_EQUAL(lerp(r1, r2, 0.5f).quaternion(), interpolatedHalf.quaternion());
	BOOST_CHECK_NE(lerp(r1, r2, 0.1f).quaternion(), interpolatedTenth.quaternion());
}

BOOST_AUTO_TEST_CASE(RotationsCanBeSphericallyInterpolated) {
	const auto r1 = Rotation(degrees(10.0f), { 1.2f, 1.0f, 3.1f });
	const auto r2 = Rotation(degrees(120.0f), { 0.2f, 1.0f, 0.3f });
	const auto interpolatedHalf = Rotation(Quat(0.851849f, { 0.109921f, 0.478132f, 0.183465f }));
	const auto interpolatedTenth = Rotation(Quat(0.986452f, { 0.047355f, 0.119935f, 0.101411f }));

	BOOST_CHECK_EQUAL(slerp(r1, r2, 0.5f).quaternion(), interpolatedHalf.quaternion());
	BOOST_CHECK_EQUAL(slerp(r1, r2, 0.1f).quaternion(), interpolatedTenth.quaternion());
}

BOOST_AUTO_TEST_SUITE_END(/* PulpMathRotationTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* PulpMathTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* PulpTestSuite */);


} // anonymous namespace
