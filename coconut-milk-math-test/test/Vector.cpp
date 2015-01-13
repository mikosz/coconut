#include <boost/test/auto_unit_test.hpp>

#include <chrono>
#include <iostream>

#include "milk/math/Vector.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::math;

namespace /* anonymous */ {

BOOST_AUTO_TEST_SUITE(MilkMathVectorTestSuite);

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

BOOST_AUTO_TEST_CASE(VectorMultiplicationPerformance) {
	Vector4d vec1(1.0f, 1.0f, 1.0f, 1.0f);

	size_t times = 100000000;

	auto start = std::chrono::monotonic_clock::now();

	for (size_t i = 0; i < times; ++i) {
		vec1 *= 10.0f;
		vec1 *= 0.1f;
	}

	auto dt = std::chrono::monotonic_clock::now() - start;
	auto millis = static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(dt).count()) / 1000.0f;

	std::cerr << "result = " << vec1 << '\n';
	std::cerr << times << " tries took " << millis << '\n';
}

BOOST_AUTO_TEST_SUITE_END(/* MilkMathVectorTestSuite */);

} // anonymous namespace
