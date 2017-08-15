#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include "coconut/pulp/math/common.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::math;

namespace /* anonymous */ {

BOOST_AUTO_TEST_SUITE(PulpTestSuite);
BOOST_AUTO_TEST_SUITE(PulpMathTestSuite);
BOOST_AUTO_TEST_SUITE(PulpMathCommonTestSuite);

BOOST_AUTO_TEST_CASE(LerpLinearlyInterpolatesBetweenValues) {
	BOOST_CHECK_EQUAL(lerp(0.0f, 4.0f, 0.25f), 1.0f);
	BOOST_CHECK_EQUAL(lerp(4.0f, 0.0f, 0.75f), 1.0f);
	BOOST_CHECK_EQUAL(lerp(0.0f, 4.0f, 0.75f), 3.0f);

	BOOST_CHECK_EQUAL(lerp(0.0, 4.0, 0.0f), 0.0);
	BOOST_CHECK_EQUAL(lerp(0.0, 4.0, 1.0f), 4.0);
}

BOOST_AUTO_TEST_SUITE_END(/* PulpMathCommonTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* PulpMathTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* PulpTestSuite */);

} // anonymous namespace
