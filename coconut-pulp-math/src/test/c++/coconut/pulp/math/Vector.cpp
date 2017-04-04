#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include "coconut/pulp/math/Vector.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::math;

namespace /* anonymous */ {

BOOST_AUTO_TEST_SUITE(PulpTestSuite);
BOOST_AUTO_TEST_SUITE(PulpMathTestSuite);
BOOST_AUTO_TEST_SUITE(PulpMathVectorTestSuite);

BOOST_AUTO_TEST_CASE(VectorConstructionAndElementAccessWorks) {
	const auto oneTwoThreeFour = Vec4(1.0f, 2.0f, 3.0f, 4.0f);
	auto oneTwoThenThree = Vector<int, 2>{ 1, 2 };

	BOOST_CHECK_EQUAL(oneTwoThreeFour.x(), 1.0f);
	BOOST_CHECK_EQUAL(oneTwoThreeFour.y(), 2.0f);
	BOOST_CHECK_EQUAL(oneTwoThreeFour.z(), 3.0f);
	BOOST_CHECK_EQUAL(oneTwoThreeFour.w(), 4.0f);

	BOOST_CHECK_EQUAL(oneTwoThenThree.x(), 1);
	BOOST_CHECK_EQUAL(oneTwoThenThree.y(), 2);
	oneTwoThenThree.y() = 4;
	BOOST_CHECK_EQUAL(oneTwoThenThree.y(), 4);
}

BOOST_AUTO_TEST_CASE(TrailingZerosAreAddedIfNecessary) {
	const auto zero = Vec2();
	const auto oneZero = Vec2(1.0f);
	const auto oneZeroIL = Vec2{ 1.0f };
	
	BOOST_CHECK_EQUAL(zero, Vec2(0.0f, 0.0f));
	BOOST_CHECK_EQUAL(oneZero, Vec2(1.0f, 0.0f));
	BOOST_CHECK_EQUAL(oneZeroIL, Vec2(1.0f, 0.0f));
}

BOOST_AUTO_TEST_CASE(VectorsAreEqualityComparable) {
	const auto iOne = Vector<int, 1>(1);
	const auto iOne2 = Vector<int, 1>(1);
	const auto iTwo = Vector<int, 1>(2);

	BOOST_CHECK_EQUAL(iOne, iOne2);
	BOOST_CHECK_NE(iOne, iTwo);

	const auto fOneTwo = Vec2(1.0f, 2.0f);
	const auto fOneAlmostTwo = Vec2(1.0f, 2.0f - 0.0000001f);
	const auto fOneNotTwo = Vec2(1.0f, 2.0f - 0.001f);

	BOOST_CHECK_EQUAL(fOneTwo, fOneAlmostTwo);
	BOOST_CHECK_NE(fOneTwo, fOneNotTwo);
}

BOOST_AUTO_TEST_CASE(VectorsAreAdditive) {
	auto lhs = Vec3(1.0f, 1.0f, 1.0f);
	const auto rhs = Vec3(0.0f, 1.0f, 2.0f);
	const auto sum = Vec3(1.0f, 2.0f, 3.0f);

	BOOST_CHECK_EQUAL(lhs + rhs, sum);

	lhs += rhs;
	BOOST_CHECK_EQUAL(lhs, sum);

	lhs -= rhs;
	BOOST_CHECK_EQUAL(lhs, sum - rhs);
}

BOOST_AUTO_TEST_CASE(VectorsAreNegatable) {
	auto vector = Vec2(4.5f, 6.7f);
	auto negated = Vec2(-4.5f, -6.7f);
	
	BOOST_CHECK_EQUAL(-vector, negated);
}

BOOST_AUTO_TEST_CASE(VectorsAreScalarMultiplicative) {
	auto vector = Vec2(0.5f, 1.0f);
	const auto product = Vec2(1.0f, 2.0f);

	BOOST_CHECK_EQUAL(vector * 2, product);

	vector *= 2.0f;
	BOOST_CHECK_EQUAL(vector, product);

	vector /= 2.0f;
	BOOST_CHECK_EQUAL(vector, product / 2.0f);
}

BOOST_AUTO_TEST_CASE(DotProducesDotProduct) {
	const auto lhs = Vec2(1.0f, 25.0f);
	const auto rhs = Vec2(0.5f, 0.1f);

	BOOST_CHECK_EQUAL(dot(lhs, rhs), 3.0f);
}

BOOST_AUTO_TEST_CASE(CrossProducesCrossProduct) {
	auto lhs = Vec3(0.123f, 4.5f, -3.14f);
	const auto rhs = Vec3(0.0f, -0.1f, 5.001f);
	const auto product = Vec3(22.1905f, -0.615123f, -0.0123f);

	BOOST_CHECK_EQUAL(cross(lhs, rhs), product);
	
	lhs.crossEq(rhs);
	BOOST_CHECK_EQUAL(lhs, product);
}

BOOST_AUTO_TEST_CASE(LengthReturnsVectorLength) {
	const auto vec = Vec2(3.12f, 0.14f);

	BOOST_CHECK_CLOSE(vec.length(), 3.12313944613f, 0.0001f);
	BOOST_CHECK_CLOSE(vec.lengthSq(), 9.754f, 0.0001f);
}

BOOST_AUTO_TEST_CASE(NormalisedReturnsNormalised) {
	auto vec = Vec2(3.12f, 0.14f);
	const auto normalised = Vec2(0.998994778f, 0.044826688f);

	BOOST_CHECK_EQUAL(vec.normalised(), normalised);
	
	vec.normalise();
	BOOST_CHECK_EQUAL(vec, normalised);

	BOOST_CHECK_CLOSE(vec.lengthSq(), 1.0f, 0.0001f);
}

BOOST_AUTO_TEST_SUITE_END(/* PulpMathVectorTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* PulpMathTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* PulpTestSuite */);

} // namespace anonymous
