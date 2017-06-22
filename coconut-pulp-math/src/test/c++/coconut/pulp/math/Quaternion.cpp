#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include "coconut/pulp/math/Quaternion.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::math;

namespace /* anonymous */ {

BOOST_AUTO_TEST_SUITE(PulpTestSuite);
BOOST_AUTO_TEST_SUITE(PulpMathTestSuite);
BOOST_AUTO_TEST_SUITE(PulpMathQuaternionTestSuite);

BOOST_AUTO_TEST_CASE(QuaternionsAreEqualityComparable) {
	const auto q = Quat(1.2f, Vec3(0.12f, 3.1f, 0.9f));
	const auto identical = Quat(1.2f, Vec3(0.12f, 3.1f, 0.9f));
	const auto close = Quat(1.2f + 0.000005f, Vec3(0.12f - 0.000005f, 3.1f, 0.9f));
	const auto differentS = Quat(1.2f + 0.0001f, Vec3(0.12f, 3.1f, 0.9f));
	const auto differentV = Quat(1.2f, Vec3(0.12f - 0.0001f, 3.1f, 0.9f));

	BOOST_CHECK_EQUAL(q, identical);
	BOOST_CHECK_EQUAL(q, close);
	BOOST_CHECK_NE(q, differentS);
	BOOST_CHECK_NE(q, differentV);
}

BOOST_AUTO_TEST_CASE(QuaternionsAreMultipliable) {
	const auto q1 = Quat(1.2f, Vec3(0.12f, 3.1f, 0.9f));
	const auto q2 = Quat(0.2f, Vec3(1.32f, 4.2f, 0.4f));
	const auto product = Quat(-13.298398f, { -0.931999f, 6.799999f, -2.927999f });

	BOOST_CHECK_EQUAL(q1 * q2, product);
}

BOOST_AUTO_TEST_CASE(QuaternionsAreScalarMultiplicative) {
	const auto q = Quat(1.2f, Vec3(0.12f, 3.1f, 0.9f));
	const auto q10 = 10.0f * q;
	const auto q01 = q / 10.0f;

	BOOST_CHECK_EQUAL(q10, Quat(12.0f, Vec3(1.2f, 31.0f, 9.0f)));
	BOOST_CHECK_EQUAL(q01, Quat(0.12f, Vec3(0.012f, 0.31f, 0.09f)));
}

BOOST_AUTO_TEST_CASE(QuaternionConjugateReturnsConjugate) {
	const auto q = Quat(1.2f, Vec3(0.12f, 3.1f, 0.9f));
	const auto c = Quat(1.2f, Vec3(-0.12f, -3.1f, -0.9f));

	BOOST_CHECK_EQUAL(q.conjugate(), c);
}

BOOST_AUTO_TEST_CASE(QuaternionNormReturnsNorm) {
	const auto q = Quat(1.2f, Vec3(0.12f, 3.1f, 0.9f));
	const auto nSq = 11.8744f;
	const auto n = 3.44592512f;

	BOOST_CHECK_CLOSE(q.normSq(), nSq, 0.01f);
	BOOST_CHECK_CLOSE(q.norm(), n, 0.01f);
}

BOOST_AUTO_TEST_CASE(QuaternionInverseReturnsInverse) {
	const auto q = Quat(1.2f, Vec3(0.12f, 3.1f, 0.9f));
	const auto inverse = Quat(0.101057f, { -0.010105f, -0.261065f, -0.075793f });

	BOOST_CHECK_EQUAL(q.inverse(), inverse);
}

BOOST_AUTO_TEST_SUITE_END(/* PulpMathQuaternionTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* PulpMathTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* PulpTestSuite */);

} // namespace anonymous
