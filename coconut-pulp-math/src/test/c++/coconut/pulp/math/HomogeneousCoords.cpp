#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include "coconut/pulp/math/HomogeneousCoords.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::math;

namespace /* anonymous */ {

BOOST_AUTO_TEST_SUITE(PulpTestSuite);
BOOST_AUTO_TEST_SUITE(PulpMathTestSuite);
BOOST_AUTO_TEST_SUITE(PulpMathHomogeneousCoordsTestSuite);

BOOST_AUTO_TEST_CASE(HomogeneousCoordsConstructibleFromVec4) {
	const auto h = HomogeneousCoords({ 1.0f, 2.0f, 3.0f, 4.0f });
	BOOST_CHECK_EQUAL(h, Vec4(1.0f, 2.0f, 3.0f, 4.0f));
}

BOOST_AUTO_TEST_CASE(HomogeneousCoordsConstructibleFromVec3Point) {
	const auto h = HomogeneousCoords({ 1.0f, 2.0f, 3.0f }, HomogeneousCoords::POINT);
	BOOST_CHECK_EQUAL(h, Vec4(1.0f, 2.0f, 3.0f, 1.0f));
}

BOOST_AUTO_TEST_CASE(HomogeneousCoordsConstructibleFromVec3Vector) {
	const auto h = HomogeneousCoords({ 1.0f, 2.0f, 3.0f }, HomogeneousCoords::VECTOR);
	BOOST_CHECK_EQUAL(h, Vec4(1.0f, 2.0f, 3.0f, 0.0f));
}

BOOST_AUTO_TEST_CASE(CanObtain3dImageOfHomogeneousCoordinates) {
	const auto p = HomogeneousCoords({ 1.0f, 2.0f, 3.0f }, HomogeneousCoords::POINT);
	const auto v = HomogeneousCoords({ 1.0f, 2.0f, 3.0f }, HomogeneousCoords::VECTOR);
	const auto h = HomogeneousCoords({ 3.0f, 3.0f, 3.0f, 3.0f });

	BOOST_CHECK_EQUAL(p.to3dSpace(), Vec3(1.0f, 2.0f, 3.0f));
	BOOST_CHECK_EQUAL(v.to3dSpace(), Vec3(1.0f, 2.0f, 3.0f));
	BOOST_CHECK_EQUAL(h.to3dSpace(), Vec3(1.0f, 1.0f, 1.0f));
}

BOOST_AUTO_TEST_SUITE_END(/* PulpMathHomogeneousCoordsTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* PulpMathTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* PulpTestSuite */);

} // namespace anonymous
