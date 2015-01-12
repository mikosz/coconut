#include <boost/test/auto_unit_test.hpp>

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

BOOST_AUTO_TEST_SUITE_END(/* MilkMathVectorTestSuite */);

} // anonymous namespace
