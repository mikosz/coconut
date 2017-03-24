#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include "coconut/pulp/math/Matrix.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::math;

namespace /* anonymous */ {

BOOST_AUTO_TEST_SUITE(PulpTestSuite);
BOOST_AUTO_TEST_SUITE(PulpMathTestSuite);
BOOST_AUTO_TEST_SUITE(PulpMathMatrixTestSuite);

BOOST_AUTO_TEST_CASE(MatrixElementAccessWorks) {
	auto mtx = Matrix4x4();

	mtx[0] = Vec4(1.0f, 2.0f, 3.0f, 4.0f);
	mtx[1] = Vec4(5.0f, 6.0f, 7.0f, 8.0f);
	mtx[2] = Vec4(9.0f, 10.0f, 11.0f, 12.0f);
	mtx[3] = Vec4(13.0f, 14.0f, 15.0f, 16.0f);

	auto element2_1 = mtx.get<2, 1>();
	BOOST_CHECK_EQUAL(element2_1, 10.0f);
	BOOST_CHECK_EQUAL(mtx.row(1), mtx[1]);
	
	const auto col2 = Vec4(3.0f, 7.0f, 11.0f, 15.0f);
	BOOST_CHECK_EQUAL(mtx.column(2), col2);
}

BOOST_AUTO_TEST_SUITE_END(/* PulpMathMatrixTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* PulpMathTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* PulpTestSuite */);

} // namespace anonymous
