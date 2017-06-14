#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include "coconut/pulp/math/Angle.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::math;

namespace /* anonymous */ {

BOOST_AUTO_TEST_SUITE(PulpTestSuite);
BOOST_AUTO_TEST_SUITE(PulpMathTestSuite);
BOOST_AUTO_TEST_SUITE(PulpMathAngleTestSuite);

BOOST_AUTO_TEST_CASE(AngleIsConstructibleWithRadians) {
	Angle a(3.0_rad);
	BOOST_CHECK_EQUAL(a.radians(), 3.0f);
}

BOOST_AUTO_TEST_CASE(AngleIsConstructibleWithDegrees) {
	Angle a(90.0_deg);
	BOOST_CHECK_EQUAL(a.degrees(), 90.0f);
}

BOOST_AUTO_TEST_CASE(AngleConvertsRadiansToDegrees) {
	Angle a(radians(PI));
	BOOST_CHECK_EQUAL(a.degrees(), 180.0f);
}

BOOST_AUTO_TEST_CASE(AngleConvertsDegreesToRadians) {
	Angle a(degrees(360.0f));
	BOOST_CHECK_EQUAL(a.radians(), 2.0f * PI);
}

BOOST_AUTO_TEST_CASE(AnglesAreAdditive) {
	Angle lhs = Angle::RIGHT;
	Angle rhs = Angle::HALF_FULL;

	BOOST_CHECK_EQUAL(lhs + rhs, radians(1.5f * PI));
	BOOST_CHECK_EQUAL(rhs - lhs, Angle::RIGHT);
}

BOOST_AUTO_TEST_CASE(AnglesAreMultiplicativeByScalar) {
	BOOST_CHECK_EQUAL(Angle::RIGHT * 4.0f, Angle::FULL);
	BOOST_CHECK_EQUAL(Angle::HALF_FULL / 2.0f, Angle::RIGHT);
}

BOOST_AUTO_TEST_CASE(AnglesAreComparable) {
	BOOST_CHECK_EQUAL(degrees(180.0f), radians(PI));
	BOOST_CHECK_NE(degrees(180.0f), degrees(180.001f));
	BOOST_CHECK_LE(degrees(180.0f), radians(PI));
	BOOST_CHECK_LT(degrees(179.0f), radians(PI));
	BOOST_CHECK_GT(radians(4.0f), degrees(180.0f));
	BOOST_CHECK_GE(radians(4.0f), degrees(180.0f));
}

BOOST_AUTO_TEST_SUITE_END(/* PulpMathAngleTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* PulpMathTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* PulpTestSuite */);

} // anonymous namespace
