#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include <DirectXMath.h>

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
	const auto m = q1 * q2;

	const auto xmq1 = DirectX::XMFLOAT4(0.12f, 3.1f, 0.9f, 1.2f);
	const auto xmq2 = DirectX::XMFLOAT4(1.32f, 4.2f, 0.4f, 0.2f);
	const auto xmv = DirectX::XMQuaternionMultiply(
		DirectX::XMLoadFloat4(&xmq2),
		DirectX::XMLoadFloat4(&xmq1)
		);
	auto xm = DirectX::XMFLOAT4();
	DirectX::XMStoreFloat4(&xm, xmv);

	BOOST_CHECK_CLOSE(m.x(), xm.x, 0.00001f);
	BOOST_CHECK_CLOSE(m.y(), xm.y, 0.00001f);
	BOOST_CHECK_CLOSE(m.z(), xm.z, 0.00001f);
	BOOST_CHECK_CLOSE(m.w(), xm.w, 0.00001f);
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

	BOOST_CHECK_CLOSE(q.normSq(), nSq, 0.00001f);
	BOOST_CHECK_CLOSE(q.norm(), n, 0.00001f);
}

BOOST_AUTO_TEST_CASE(QuaternionInverseReturnsInverse) {
	const auto q = Quat(1.2f, Vec3(0.12f, 3.1f, 0.9f));
	const auto i = q.inverse();

	const auto xmq = DirectX::XMFLOAT4(0.12f, 3.1f, 0.9f, 1.2f);
	const auto xmi = DirectX::XMQuaternionInverse(DirectX::XMLoadFloat4(&xmq));
	auto xi = DirectX::XMFLOAT4();
	DirectX::XMStoreFloat4(&xi, xmi);

	BOOST_CHECK_CLOSE(i.x(), xi.x, 0.00001f);
	BOOST_CHECK_CLOSE(i.y(), xi.y, 0.00001f);
	BOOST_CHECK_CLOSE(i.z(), xi.z, 0.00001f);
	BOOST_CHECK_CLOSE(i.w(), xi.w, 0.00001f);
}

BOOST_AUTO_TEST_SUITE_END(/* PulpMathQuaternionTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* PulpMathTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* PulpTestSuite */);

} // namespace anonymous
