#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include "coconut/pulp/math/Frustum.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::math;

namespace /* anonymous */ {

BOOST_AUTO_TEST_SUITE(PulpTestSuite);
BOOST_AUTO_TEST_SUITE(PulpMathTestSuite);
BOOST_AUTO_TEST_SUITE(PulpMathFrustumTestSuite);

BOOST_AUTO_TEST_CASE(FrustumCanTestsPointContained) {
	{
		const auto f = Frustum(Handedness::RIGHT, 1.0f, 0.5f, 0.1f, 100.0f);

		BOOST_CHECK(f.contains({ 0.0f, 0.0f, -50.0f }));

		BOOST_CHECK(f.contains({ 0.0f, 0.0f, -0.1f }));
		BOOST_CHECK(f.contains({ -1.0f, 0.0f, -1.0f }));
		BOOST_CHECK(f.contains({ 1.0f, 0.0f, -1.0f }));
		BOOST_CHECK(f.contains({ 0.0f, 0.0f, -100.0f }));

		BOOST_CHECK(!f.contains({ 0.0f, 0.0f, -0.09f }));
		BOOST_CHECK(!f.contains({ -1.1f, 0.0f, -1.0f }));
		BOOST_CHECK(!f.contains({ 1.1f, 0.0f, -1.0f }));
		BOOST_CHECK(!f.contains({ 0.0f, 0.0f, -100.1f }));
	}

	{
		const auto f = Frustum(Handedness::LEFT, 1.0f, 0.5f, 0.1f, 100.0f);

		BOOST_CHECK(f.contains({ 0.0f, 0.0f, 50.0f }));

		BOOST_CHECK(f.contains({ 0.0f, 0.0f, 0.1f }));
		BOOST_CHECK(f.contains({ -1.0f, 0.0f, 1.0f }));
		BOOST_CHECK(f.contains({ 1.0f, 0.0f, 1.0f }));
		BOOST_CHECK(f.contains({ 0.0f, 0.0f, 100.0f }));

		BOOST_CHECK(!f.contains({ 0.0f, 0.0f, 0.09f }));
		BOOST_CHECK(!f.contains({ -1.1f, 0.0f, 1.0f }));
		BOOST_CHECK(!f.contains({ 1.1f, 0.0f, 1.0f }));
		BOOST_CHECK(!f.contains({ 0.0f, 0.0f, 100.1f }));
	}
}

BOOST_AUTO_TEST_SUITE_END(/* PulpMathFrustumTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* PulpMathTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* PulpTestSuite */);

} // anonymous namespace
