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
	const auto oneTwoThreeFour = Vec4{ 1.0f, 2.0f, 3, 4.0f };
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

BOOST_AUTO_TEST_CASE(VectorsAreEqualityComparable) {
	const auto iOne = Vector<int, 1>{ 1 };
	const auto iOne2 = Vector<int, 1>{ 1 };
	const auto iTwo = Vector<int, 1>{ 2 };

	BOOST_CHECK_EQUAL(iOne, iOne2);
	BOOST_CHECK_NE(iOne, iTwo);

	const auto fOneTwo = Vec2{ 1.0f, 2.0f };
	const auto fOneAlmostTwo = Vec2{ 1.0f, 2.0f - 0.0000001f };
	const auto fOneNotTwo = Vec2{ 1.0f, 2.0f - 0.001f };

	BOOST_CHECK_EQUAL(fOneTwo, fOneAlmostTwo);
	BOOST_CHECK_NE(fOneTwo, fOneNotTwo);
}

BOOST_AUTO_TEST_CASE(VectorsAreAdditive) {
	auto lhs = Vec3{ 1.0f, 1.0f, 1.0f };
	const auto rhs = Vec3{ 0.0f, 1.0f, 2.0f };
	const auto sum = Vec3{ 1.0f, 2.0f, 3.0f };

	BOOST_CHECK_EQUAL(lhs + rhs, sum);

	lhs += rhs;
	BOOST_CHECK_EQUAL(lhs, sum);

	lhs -= rhs;
	BOOST_CHECK_EQUAL(lhs, sum - rhs);
}

BOOST_AUTO_TEST_CASE(VectorsAreNegatable) {
	auto vector = Vec2{ 4.5f, 6.7f };
	auto negated = Vec2{ -4.5f, -6.7f };
	
	BOOST_CHECK_EQUAL(-vector, negated);
}

BOOST_AUTO_TEST_CASE(VectorsAreScalarMultiplicative) {
	auto vector = Vec2{ 0.5f, 1.0f };
	const auto product = Vec2{ 1.0f, 2.0f };

	BOOST_CHECK_EQUAL(vector * 2, product);

	vector *= 2.0f;
	BOOST_CHECK_EQUAL(vector, product);

	vector /= 2.0f;
	BOOST_CHECK_EQUAL(vector, product / 2.0f);
}

BOOST_AUTO_TEST_CASE(DotProducesDotProduct) {
	const auto lhs = Vec2{ 1.0f, 25.0f };
	const auto rhs = Vec2{ 0.5f, 0.1f };

	BOOST_CHECK_EQUAL(dot(lhs, rhs), 3.0f);
}

BOOST_AUTO_TEST_CASE(CrossProducesCrossProduct) {
	auto lhs = Vec3{ 0.123f, 4.5f, -3.14f };
	const auto rhs = Vec3{ 0.0f, -0.1f, 5.001f };
	const auto product = Vec3{ 22.1905f, -0.615123f, -0.0123f };

	BOOST_CHECK_EQUAL(cross(lhs, rhs), product);
	
	lhs.crossEq(rhs);
	BOOST_CHECK_EQUAL(lhs, product);
}

BOOST_AUTO_TEST_CASE(LengthReturnsVectorLength) {
	const auto vec = Vec2{ 3.12f, 0.14f };

	BOOST_CHECK_CLOSE(vec.length(), 3.12313944613f, 0.0001f);
	BOOST_CHECK_CLOSE(vec.lengthSq(), 9.754f, 0.0001f);
}

BOOST_AUTO_TEST_CASE(NormalisedReturnsNormalised) {
	auto vec = Vec2{ 3.12f, 0.14f };
	const auto normalised = Vec2{ 0.998994778f, 0.044826688f };

	BOOST_CHECK_EQUAL(vec.normalised(), normalised);
	
	vec.normalise();
	BOOST_CHECK_EQUAL(vec, normalised);

	BOOST_CHECK_CLOSE(vec.lengthSq(), 1.0f, 0.0001f);
}

BOOST_AUTO_TEST_SUITE_END(/* PulpMathVectorTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* PulpMathTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* PulpTestSuite */);

} // namespace anonymous

#if 0
#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include "coconut/pulp/math/Vector.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::math;

namespace /* anonymous */ {

BOOST_AUTO_TEST_SUITE(MilkMathVectorTestSuite);

BOOST_AUTO_TEST_CASE(ZeroVectorEqualsZeroAtAllElements) {
	const auto zero = Vector<3>::ZERO;

	BOOST_CHECK_EQUAL(zero.get<0>(), 0.0f);
	BOOST_CHECK_EQUAL(zero.get<1>(), 0.0f);
	BOOST_CHECK_EQUAL(zero.get<2>(), 0.0f);
}

BOOST_AUTO_TEST_CASE(VectorsEqualIfElementsDifferByLessThanEpsilon) {
	Vector<3> lhs;
	lhs.get<0>() = 0.0f;
	lhs.get<1>() = 1.0f;
	lhs.get<2>() = 2.0f;

	Vector<3> rhs;
	rhs.get<0>() = 0.0f + 0.00009f;
	rhs.get<1>() = 1.0f;
	rhs.get<2>() = 2.0f - 0.00009f;

	BOOST_CHECK_EQUAL(lhs, rhs);
}

BOOST_AUTO_TEST_CASE(VectorsNotEqualIfElementOffByMoreThanEpsilon) {
	Vector<3> lhs;
	lhs.get<0>() = 0.0f;
	lhs.get<1>() = 1.0f;
	lhs.get<2>() = 2.0f;

	Vector<3> rhs;
	rhs.get<0>() = 0.0f + 0.00011f;
	rhs.get<1>() = 1.0f;
	rhs.get<2>() = 2.0f;

	BOOST_CHECK_NE(lhs, rhs);

	rhs.get<0>() = 0.0f;
	rhs.get<1>() = 1.0f;
	rhs.get<2>() = 2.0f - 0.00011f;

	BOOST_CHECK_NE(lhs, rhs);
}

BOOST_AUTO_TEST_CASE(VectorElementsAccessible) {
	Vector<4> vector;
	vector.get<0>() = 0.0f;
	vector.get<1>() = 1.0f;
	vector.get<2>() = 2.0f;
	vector.get<3>() = 3.0f;

	BOOST_CHECK_EQUAL(vector.get<0>(), 0);
	BOOST_CHECK_EQUAL(vector.get<1>(), 1);
	BOOST_CHECK_EQUAL(vector.get<2>(), 2);
	BOOST_CHECK_EQUAL(vector.get<3>(), 3);
}

BOOST_AUTO_TEST_CASE(Vector1dHasXAccessor) {
	Vector1d vector;
	vector.x() = 0.0f;

	BOOST_CHECK_EQUAL(vector.x(), 0.0f);
}

BOOST_AUTO_TEST_CASE(Vector2dHasXYAccessors) {
	Vector2d vector;
	vector.x() = 0.0f;
	vector.y() = 1.0f;

	BOOST_CHECK_EQUAL(vector.x(), 0.0f);
	BOOST_CHECK_EQUAL(vector.y(), 1.0f);
}

BOOST_AUTO_TEST_CASE(Vector3dHasXYZAccessors) {
	Vector3d vector;
	vector.x() = 0.0f;
	vector.y() = 1.0f;
	vector.z() = 2.0f;

	BOOST_CHECK_EQUAL(vector.x(), 0.0f);
	BOOST_CHECK_EQUAL(vector.y(), 1.0f);
	BOOST_CHECK_EQUAL(vector.z(), 2.0f);
}

BOOST_AUTO_TEST_CASE(Vector4dHasXYZWAccessors) {
	Vector4d vector;
	vector.x() = 0.0f;
	vector.y() = 1.0f;
	vector.z() = 2.0f;
	vector.w() = 3.0f;

	BOOST_CHECK_EQUAL(vector.x(), 0.0f);
	BOOST_CHECK_EQUAL(vector.y(), 1.0f);
	BOOST_CHECK_EQUAL(vector.z(), 2.0f);
	BOOST_CHECK_EQUAL(vector.w(), 3.0f);
}

BOOST_AUTO_TEST_CASE(Vectors1dAreComparable) {
	Vector1d vector(1.23f);
	Vector1d equal(1.23f);
	Vector1d notEqual(1.33f);

	BOOST_CHECK_EQUAL(vector, equal);
	BOOST_CHECK_NE(vector, notEqual);
}

BOOST_AUTO_TEST_CASE(Vectors2dAreComparable) {
	Vector2d vector(1.23f, 4.56f);
	Vector2d equal(1.23f, 4.56f);
	Vector2d notEqual(1.23f, 4.57f);

	BOOST_CHECK_EQUAL(vector, equal);
	BOOST_CHECK_NE(vector, notEqual);
}

BOOST_AUTO_TEST_CASE(Vectors3dAreComparable) {
	Vector3d vector(1.23f, 4.56f, 7.89f);
	Vector3d equal(1.23f, 4.56f, 7.89f);
	Vector3d notEqual(1.23f, 4.56f, 7.99f);

	BOOST_CHECK_EQUAL(vector, equal);
	BOOST_CHECK_NE(vector, notEqual);
}

BOOST_AUTO_TEST_CASE(Vectors4AreComparable) {
	Vector4d vector(1.23f, 4.56f, 7.89f, 0.12f);
	Vector4d equal(1.23f, 4.56f, 7.89f, 0.12f);
	Vector4d notEqual(1.23f, 4.56f, 7.89f, 0.13f);

	BOOST_CHECK_EQUAL(vector, equal);
	BOOST_CHECK_NE(vector, notEqual);
}

BOOST_AUTO_TEST_CASE(Vectors1dAreMultiplicativeByScalar) {
	Vector1d vector(1.0f);

	BOOST_CHECK_EQUAL(vector * 3.0f, Vector1d(3.0f));
	BOOST_CHECK_EQUAL(vector / 3.0f, Vector1d(1.0f / 3.0f));
	BOOST_CHECK_EQUAL(-vector, Vector1d(-1.0f));
}

BOOST_AUTO_TEST_CASE(Vectors2dAreMultiplicativeByScalar) {
	Vector2d vector(1.0f, 2.0f);

	BOOST_CHECK_EQUAL(vector * 3.0f, Vector2d(3.0f, 6.0f));
	BOOST_CHECK_EQUAL(vector / 3.0f, Vector2d(1.0f / 3.0f, 2.0f / 3.0f));
	BOOST_CHECK_EQUAL(-vector, Vector2d(-1.0f, -2.0f));
}

BOOST_AUTO_TEST_CASE(Vectors3dAreMultiplicativeByScalar) {
	Vector3d vector(1.0f, 2.0f, 3.0f);

	BOOST_CHECK_EQUAL(vector * 3.0f, Vector3d(3.0f, 6.0f, 9.0f));
	BOOST_CHECK_EQUAL(vector / 3.0f, Vector3d(1.0f / 3.0f, 2.0f / 3.0f, 3.0f / 3.0f));
	BOOST_CHECK_EQUAL(-vector, Vector3d(-1.0f, -2.0f, -3.0f));
}

BOOST_AUTO_TEST_CASE(Vectors4dAreMultiplicativeByScalar) {
	Vector4d vector(1.0f, 2.0f, 3.0f, 4.0f);

	BOOST_CHECK_EQUAL(vector * 3.0f, Vector4d(3.0f, 6.0f, 9.0f, 12.0f));
	BOOST_CHECK_EQUAL(vector / 3.0f, Vector4d(1.0f / 3.0f, 2.0f / 3.0f, 3.0f / 3.0f, 4.0f / 3.0f));
	BOOST_CHECK_EQUAL(-vector, Vector4d(-1.0f, -2.0f, -3.0f, -4.0f));
}

BOOST_AUTO_TEST_CASE(Vectors1dAreDotMultiplicative) {
	Vector1d lhs(2.0f);
	Vector1d rhs(3.0f);

	BOOST_CHECK_EQUAL(lhs.dot(rhs), 6.0f);
}

BOOST_AUTO_TEST_CASE(Vectors2dAreDotMultiplicative) {
	Vector2d lhs(2.0f, 3.0f);
	Vector2d rhs(3.0f, 4.0f);

	BOOST_CHECK_EQUAL(lhs.dot(rhs), 18.0f);
	BOOST_CHECK_EQUAL(dot(lhs, rhs), 18.0f);
}

BOOST_AUTO_TEST_CASE(Vectors3dAreDotMultiplicative) {
	Vector3d lhs(2.0f, 3.0f, 4.0f);
	Vector3d rhs(3.0f, 4.0f, 5.0f);

	BOOST_CHECK_EQUAL(lhs.dot(rhs), 38.0f);
	BOOST_CHECK_EQUAL(dot(lhs, rhs), 38.0f);
}

BOOST_AUTO_TEST_CASE(Vectors4dAreDotMultiplicative) {
	Vector4d lhs(2.0f, 3.0f, 4.0f, 5.0f);
	Vector4d rhs(3.0f, 4.0f, 5.0f, 6.0f);

	BOOST_CHECK_EQUAL(lhs.dot(rhs), 68.0f);
	BOOST_CHECK_EQUAL(dot(lhs, rhs), 68.0f);
}

BOOST_AUTO_TEST_CASE(Vectors3dAreCrossMultiplicative) {
	Vector3d lhs(2.0f, 3.0f, 4.0f);
	Vector3d rhs(0.4f, 0.3f, 0.2f);

	BOOST_CHECK_EQUAL(lhs.cross(rhs), Vector3d(-0.6f, 1.2f, -0.6f));
	BOOST_CHECK_EQUAL(cross(lhs, rhs), Vector3d(-0.6f, 1.2f, -0.6f));

	BOOST_CHECK_EQUAL(lhs.crossEq(rhs), Vector3d(-0.6f, 1.2f, -0.6f));
	BOOST_CHECK_EQUAL(lhs, Vector3d(-0.6f, 1.2f, -0.6f));
}

BOOST_AUTO_TEST_CASE(Vectors1dAreAdditive) {
	Vector1d lhs(1.0f);
	Vector1d rhs(2.0f);

	BOOST_CHECK_EQUAL(lhs + rhs, Vector1d(3.0f));
	BOOST_CHECK_EQUAL(lhs - rhs, Vector1d(-1.0f));
}

BOOST_AUTO_TEST_CASE(Vectors2dAreAdditive) {
	Vector2d lhs(1.0f, 2.0f);
	Vector2d rhs(2.0f, 3.0f);

	BOOST_CHECK_EQUAL(lhs + rhs, Vector2d(3.0f, 5.0f));
	BOOST_CHECK_EQUAL(lhs - rhs, Vector2d(-1.0f, -1.0f));
}

BOOST_AUTO_TEST_CASE(Vectors3dAreAdditive) {
	Vector3d lhs(1.0f, 2.0f, 3.0f);
	Vector3d rhs(2.0f, 3.0f, 4.0f);

	BOOST_CHECK_EQUAL(lhs + rhs, Vector3d(3.0f, 5.0f, 7.0f));
	BOOST_CHECK_EQUAL(lhs - rhs, Vector3d(-1.0f, -1.0f, -1.0f));
}

BOOST_AUTO_TEST_CASE(Vectors4dAreAdditive) {
	Vector4d lhs(1.0f, 2.0f, 3.0f, 4.0f);
	Vector4d rhs(2.0f, 3.0f, 4.0f, 5.0f);

	BOOST_CHECK_EQUAL(lhs + rhs, Vector4d(3.0f, 5.0f, 7.0f, 9.0f));
	BOOST_CHECK_EQUAL(lhs - rhs, Vector4d(-1.0f, -1.0f, -1.0f, -1.0f));
}

BOOST_AUTO_TEST_CASE(Vectors1dAreMeasurable) {
	Vector1d vector(1.0f);
	
	BOOST_CHECK_EQUAL(vector.length(), 1.0f);
	BOOST_CHECK_EQUAL(length(vector), 1.0f);
}

BOOST_AUTO_TEST_CASE(Vectors2dAreMeasurable) {
	Vector2d vector(1.0f, 2.0f);

	BOOST_CHECK_EQUAL(vector.length(), std::sqrt(5.0f));
	BOOST_CHECK_EQUAL(length(vector), std::sqrt(5.0f));
}

BOOST_AUTO_TEST_CASE(Vectors3dAreMeasurable) {
	Vector3d vector(1.0f, 2.0f, 3.0f);

	BOOST_CHECK_EQUAL(vector.length(), std::sqrt(14.0f));
	BOOST_CHECK_EQUAL(length(vector), std::sqrt(14.0f));
}

BOOST_AUTO_TEST_CASE(Vectors4dAreMeasurable) {
	Vector4d vector(1.0f, 2.0f, 3.0f, 4.0f);

	BOOST_CHECK_EQUAL(vector.length(), std::sqrt(30.0f));
	BOOST_CHECK_EQUAL(length(vector), std::sqrt(30.0f));
}

BOOST_AUTO_TEST_SUITE_END(/* MilkMathVectorTestSuite */);

} // anonymous namespace
#endif
