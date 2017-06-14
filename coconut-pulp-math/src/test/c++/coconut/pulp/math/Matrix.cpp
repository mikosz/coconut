#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include <coconut-tools/utils/Range.hpp>

#include "coconut/pulp/math/Matrix.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::math;

namespace /* anonymous */ {

BOOST_AUTO_TEST_SUITE(PulpTestSuite);
BOOST_AUTO_TEST_SUITE(PulpMathTestSuite);
BOOST_AUTO_TEST_SUITE(PulpMathMatrixTestSuite);

BOOST_AUTO_TEST_CASE(TransposedMatrixViewWorks) {
	auto mtx = Matrix4x4();

	mtx[0] = Vec4(1.0f, 2.0f, 3.0f, 4.0f);
	mtx[1] = Vec4(5.0f, 6.0f, 7.0f, 8.0f);
	mtx[2] = Vec4(9.0f, 10.0f, 11.0f, 12.0f);
	mtx[3] = Vec4(13.0f, 14.0f, 15.0f, 16.0f);

	auto view = viewMatrixTransposed(mtx);

	BOOST_CHECK_EQUAL(view.get(2, 1), mtx[1][2]);

	view.get(2, 1) = 42.0f;
	BOOST_CHECK_EQUAL(mtx[1][2], 42.0f);
}

BOOST_AUTO_TEST_CASE(SubmatrixViewWorks) {
	auto mtx = Matrix4x4();

	mtx[0] = Vec4(1.0f, 2.0f, 3.0f, 4.0f);
	mtx[1] = Vec4(5.0f, 6.0f, 7.0f, 8.0f);
	mtx[2] = Vec4(9.0f, 10.0f, 11.0f, 12.0f);
	mtx[3] = Vec4(13.0f, 14.0f, 15.0f, 16.0f);

	auto view = viewSubmatrix(mtx, 1, 2);

	BOOST_CHECK_EQUAL(view.get(0, 1), mtx[0][1]);
	BOOST_CHECK_EQUAL(view.get(1, 1), mtx[2][1]);
	BOOST_CHECK_EQUAL(view.get(0, 2), mtx[0][3]);
	BOOST_CHECK_EQUAL(view.get(1, 2), mtx[2][3]);
}

BOOST_AUTO_TEST_CASE(ViewChainingWorks) {
	auto mtx = Matrix4x4();

	mtx[0] = Vec4(1.0f, 2.0f, 3.0f, 4.0f);
	mtx[1] = Vec4(5.0f, 6.0f, 7.0f, 8.0f);
	mtx[2] = Vec4(9.0f, 10.0f, 11.0f, 12.0f);
	mtx[3] = Vec4(13.0f, 14.0f, 15.0f, 16.0f);

	auto view = submatrix(viewMatrixTransposed(mtx), 1, 2);

	BOOST_CHECK_EQUAL(view.get(0, 2), mtx[3][0]);
}

BOOST_AUTO_TEST_CASE(MatrixIsConstructibleFromVectors) {
	auto mtx = Matrix4x4(
		Vec4(1.0f, 2.0f, 3.0f, 4.0f),
		Vec4(5.0f, 6.0f, 7.0f, 8.0f),
		Vec4(9.0f, 10.0f, 11.0f, 12.0f),
		Vec4(13.0f, 14.0f, 15.0f, 16.0f)
		);

	for (const auto rowIndex : coconut_tools::range(0, 4)) {
		for (const auto columnIndex : coconut_tools::range(0, 4)) {
			const auto expected = (rowIndex * 4.0f) + columnIndex + 1.0f;
			BOOST_CHECK_EQUAL(mtx[rowIndex][columnIndex], expected);
		}
	}
}

BOOST_AUTO_TEST_CASE(MatrixIsConstructibleFromInitialiserLists) {
	auto mtx = Matrix4x4(
		{ 1.0f, 2.0f, 3.0f, 4.0f },
		{ 5.0f, 6.0f, 7.0f, 8.0f },
		{ 9.0f, 10.0f, 11.0f, 12.0f },
		{ 13.0f, 14.0f, 15.0f, 16.0f }
		);

	for (const auto rowIndex : coconut_tools::range(0, 4)) {
		for (const auto columnIndex : coconut_tools::range(0, 4)) {
			const auto expected = (rowIndex * 4.0f) + columnIndex + 1.0f;
			BOOST_CHECK_EQUAL(mtx[rowIndex][columnIndex], expected);
		}
	}
}

BOOST_AUTO_TEST_CASE(DefaultMatrixConstructorYieldsZeroes) {
	auto mtx = Matrix4x4();

	for (const auto rowIndex : coconut_tools::range(0, 4)) {
		for (const auto columnIndex : coconut_tools::range(0, 4)) {
			BOOST_CHECK_EQUAL(mtx[rowIndex][columnIndex], 0.0f);
		}
	}
}

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

	auto rowModified = mtx;

	rowModified.row(2) = rowModified.row(0);
	mtx[2] = mtx[0];

	BOOST_CHECK_EQUAL(mtx, rowModified);
}

BOOST_AUTO_TEST_CASE(MatricesAreEqualityComparible) {
	auto mtx = Matrix4x4();

	mtx[0] = Vec4(1.0f, 2.0f, 3.0f, 4.0f);
	mtx[1] = Vec4(5.0f, 6.0f, 7.0f, 8.0f);
	mtx[2] = Vec4(9.0f, 10.0f, 11.0f, 12.0f);
	mtx[3] = Vec4(13.0f, 14.0f, 15.0f, 16.0f);

	const auto other = mtx;

	BOOST_CHECK_EQUAL(mtx, other);

	mtx[1][2] = 42.0f;
	BOOST_CHECK_NE(mtx, other);
}

BOOST_AUTO_TEST_CASE(MatricesAreAdditive) {
	using IMatrix2x1 = math::Matrix<int, 2, 1>;
	auto lhs = IMatrix2x1();
	lhs[0] = IMatrix2x1::Row(1);
	lhs[1] = IMatrix2x1::Row(2);

	auto rhs = IMatrix2x1();
	rhs[0] = IMatrix2x1::Row(2);
	rhs[1] = IMatrix2x1::Row(1);

	auto sum = IMatrix2x1();
	sum[0] = IMatrix2x1::Row(3);
	sum[1] = IMatrix2x1::Row(3);

	auto difference = IMatrix2x1();
	difference[0] = IMatrix2x1::Row(-1);
	difference[1] = IMatrix2x1::Row(1);

	BOOST_CHECK_EQUAL(lhs + rhs, sum);
	BOOST_CHECK_EQUAL(lhs - rhs, difference);

	lhs += rhs;
	BOOST_CHECK_EQUAL(lhs, sum);

	lhs -= rhs + rhs;
	BOOST_CHECK_EQUAL(lhs, difference);
}

BOOST_AUTO_TEST_CASE(MatricesAreNegatable) {
	using IMatrix2x1 = math::Matrix<int, 2, 1>;
	auto matrix = IMatrix2x1();
	matrix[0] = IMatrix2x1::Row(1);
	matrix[1] = IMatrix2x1::Row(2);

	auto negated = IMatrix2x1();
	negated[0] = IMatrix2x1::Row(-1);
	negated[1] = IMatrix2x1::Row(-2);

	BOOST_CHECK_EQUAL(-matrix, negated);
}

BOOST_AUTO_TEST_CASE(MatricesAreScalarMultiplicative) {
	using Matrix2x1 = math::Matrix<float, 2, 1>;
	auto matrix = Matrix2x1();
	matrix[0] = Matrix2x1::Row(1.0f);
	matrix[1] = Matrix2x1::Row(2.0f);

	auto twice = Matrix2x1();
	twice[0] = Matrix2x1::Row(2.0f);
	twice[1] = Matrix2x1::Row(4.0f);

	auto half = Matrix2x1();
	half[0] = Matrix2x1::Row(0.5f);
	half[1] = Matrix2x1::Row(1.0f);
	
	BOOST_CHECK_EQUAL(matrix * 2.0f, twice);
	BOOST_CHECK_EQUAL(matrix / 2.0f, half);
}

BOOST_AUTO_TEST_CASE(MatricesAreMultipliable) {
	using Matrix2x3 = math::Matrix<float, 2, 3>;
	auto lhs = Matrix2x3();
	lhs[0] = Matrix2x3::Row(1.0f, 0.042f, 3.5f);
	lhs[1] = Matrix2x3::Row(2.0f, 0.0001f, 1.2f);

	using Matrix3x2 = math::Matrix<float, 3, 2>;
	auto rhs = Matrix3x2();
	rhs[0] = Matrix3x2::Row(5.6f, 0.02f);
	rhs[1] = Matrix3x2::Row(2.0f, 8.0f);
	rhs[2] = Matrix3x2::Row(0.0f, 0.56f);

	using Matrix2x2 = math::Matrix<float, 2, 2>;
	auto result = Matrix2x2();
	result[0] = Matrix2x2::Row(5.684f, 2.316f);
	result[1] = Matrix2x2::Row(11.2002f, 0.7128f);

	BOOST_CHECK_EQUAL(lhs * rhs, result);
}

BOOST_AUTO_TEST_CASE(IdentityMatrixIsIdentity) {
	using IMatrix2x3 = math::Matrix<int, 2, 3>;
	auto identity = IMatrix2x3();
	identity[0] = IMatrix2x3::Row(1, 0, 0);
	identity[1] = IMatrix2x3::Row(0, 1, 0);

	BOOST_CHECK_EQUAL(identity, IMatrix2x3::IDENTITY);
}

BOOST_AUTO_TEST_CASE(TransposeReturnsTranspose) {
	using IMatrix2x3 = math::Matrix<int, 2, 3>;
	auto mtx2x3 = IMatrix2x3();
	mtx2x3[0] = IMatrix2x3::Row(1, 3, 5);
	mtx2x3[1] = IMatrix2x3::Row(2, 4, 6);

	const auto transpose = mtx2x3.transpose();
	
	BOOST_CHECK_EQUAL(decltype(transpose)::ROWS, 3);
	BOOST_CHECK_EQUAL(decltype(transpose)::COLUMNS, 2);
	BOOST_CHECK_EQUAL(transpose.row(0), mtx2x3.column(0));
	BOOST_CHECK_EQUAL(transpose.row(1), mtx2x3.column(1));
	BOOST_CHECK_EQUAL(transpose.row(2), mtx2x3.column(2));
}

BOOST_AUTO_TEST_CASE(DeterminantReturnsDeterminant) {
	using IMatrix3x3 = math::Matrix<int, 3, 3>;
	auto mtx3x3 = IMatrix3x3();
	mtx3x3[0] = IMatrix3x3::Row(1, -3, 5);
	mtx3x3[1] = IMatrix3x3::Row(2, 2, 2);
	mtx3x3[2] = IMatrix3x3::Row(0, -10, 2);

	BOOST_CHECK_EQUAL(mtx3x3.determinant(), -64);
}

BOOST_AUTO_TEST_CASE(InverseReturnsInverse) {
	auto mtx = Matrix4x4();
	mtx[0] = Vec4(1.14f, 0.2f, 0.02f, 0.1f);
	mtx[1] = Vec4(-0.42f, 2.0f, -0.07f, 0.2f);
	mtx[2] = Vec4(-0.11f, 0.12f, 2.12f, 0.3f);
	mtx[3] = Vec4(0.3f, 0.5f, 1.2f, 0.1f);

	auto inverse = Matrix4x4();
	inverse[0] = Vec4(0.6738507691955666f, -0.16325918484540142f, -0.264793054785559f, 0.44704676485191314f);
	inverse[1] = Vec4(-0.13578742231611368f, 0.33564346090725455f, -0.45189877709993576f, 0.8201968318014119f);
	inverse[2] = Vec4(-0.33324502303465253f, -0.20155148605280812f, -0.07360723126261765f, 0.9571696889281217f);
	inverse[3] = Vec4(2.656325080409698f, 1.2301780826336286f, 3.9371598250077673f, -6.928160720700258f);

	BOOST_CHECK_EQUAL(mtx.inverse(), inverse);
	BOOST_CHECK_EQUAL(mtx.inverse() * mtx, mtx * mtx.inverse());
	BOOST_CHECK_EQUAL(mtx.inverse() * mtx, Matrix4x4::IDENTITY);
}

BOOST_AUTO_TEST_SUITE_END(/* PulpMathMatrixTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* PulpMathTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* PulpTestSuite */);

} // namespace anonymous
