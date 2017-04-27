#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include "coconut/pulp/math/Plane.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::math;

namespace /* anonymous */ {

BOOST_AUTO_TEST_SUITE(PulpTestSuite);
BOOST_AUTO_TEST_SUITE(PulpMathTestSuite);
BOOST_AUTO_TEST_SUITE(PulpMathPlaneTestSuite);

BOOST_AUTO_TEST_CASE(PlaneIsConstructibleFromPointAndNormal) {
	const auto p = Plane({ 0.0f, 1.0f, 0.0f }, { 2.0f, 2.0f, 2.0f });

	BOOST_CHECK_EQUAL(p.normal(), Vec3(0.0f, 1.0f, 0.0f).normalised());
	BOOST_CHECK_CLOSE(p.signedDistanceToOrigin(), -2.0f, 0.00001f);
}

BOOST_AUTO_TEST_CASE(PlaneIsConstructibleFromNormalAndDistanceFromOrigin) {

}

BOOST_AUTO_TEST_CASE(CanTestPointPositionComparedToPlane) {
	const auto p = Plane({ 1.0f, 1.0f, 1.0f }, { 1.0f, -1.0f, 1.0f });

	BOOST_CHECK(p.testPosition({ 1.0f, 0.0f, 1.0f }) == Plane::Side::FRONT);
	BOOST_CHECK(p.testPosition({ 1.0f, -1.0f, 1.0f }) == Plane::Side::ON);
	BOOST_CHECK(p.testPosition({ 1.0f, -2.0f, 1.0f }) == Plane::Side::BACK);
}

BOOST_AUTO_TEST_SUITE_END(/* PulpMathPlaneTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* PulpMathTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* PulpTestSuite */);

} // anonymous namespace
