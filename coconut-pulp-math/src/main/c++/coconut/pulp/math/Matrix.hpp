#ifndef _COCONUT_PULP_MATH_MATRIX_HPP_
#define _COCONUT_PULP_MATH_MATRIX_HPP_

#include <array>
#include <cmath>
#include <type_traits>
#include <iosfwd>
#include <algorithm>
#include <functional>

#include <boost/operators.hpp>

#include "coconut-tools/utils/InfixOstreamIterator.hpp"

#include "Angle.hpp"
#include "Handedness.hpp"
#include "Vector.hpp"
#include "ScalarEqual.hpp"

namespace coconut {
namespace pulp {
namespace math {

template <class NextViewType, class GetElementFunc>
class MatrixView {
public:

	using MatrixType = typename GetElementFunc::MatrixType;

	using Scalar = typename MatrixType::Scalar;

	static const auto ROWS = GetElementFunc::ROWS;

	static const auto COLUMNS = GetElementFunc::COLUMNS;

	constexpr MatrixView(NextViewType nextView, GetElementFunc getElementFunc = GetElementFunc()) :
		next_(std::move(nextView)),
		getElementFunc_(std::move(getElementFunc))
	{
	}

	constexpr auto get(size_t row, size_t column) const noexcept -> decltype(auto) {
		return getElementFunc_(next_, row, column);
	}

	template <size_t ROWS_ = ROWS>
	auto determinant() const noexcept {
		static_assert(ROWS_ == ROWS, "Rows count changed");
		static_assert(ROWS == COLUMNS, "Determinant only available for square matrices");

		auto result = MatrixType::Scalar(0);
		for (size_t columnIndex = 0; columnIndex < COLUMNS; ++columnIndex) {
			const auto absElement = get(0, columnIndex) * submatrix(*this, 0, columnIndex).determinant();
			if (columnIndex % 2 == 0) {
				result += absElement;
			} else {
				result -= absElement;
			}
		}
		return result;
	}

	template <>
	auto determinant<1>() const noexcept {
		static_assert(ROWS == COLUMNS, "Determinant only available for square matrices");
		return get(0, 0);
	}

	auto cofactor(size_t rowIndex, size_t columnIndex) const noexcept {
		const auto det = submatrix(*this, rowIndex, columnIndex).determinant();
		return ((rowIndex + columnIndex) % 2 == 0) ? det : -det;
	}

	template <size_t ROWS_ = ROWS>
	auto inverse() const noexcept {
		static_assert(ROWS_ == ROWS, "Rows count changed");
		static_assert(ROWS == COLUMNS, "Inverse only available for square matrices");

		const auto det = determinant();
		assert(det != Scalar(0));
		const auto detInverse = Scalar(1) / det;

		auto result = MatrixType();

		for (size_t rowIndex = 0; rowIndex < COLUMNS; ++rowIndex) {
			for (size_t columnIndex = 0; columnIndex < COLUMNS; ++columnIndex) {
				result[rowIndex][columnIndex] = detInverse * cofactor(columnIndex, rowIndex);
			}
		}

		return result;
	}

private:

	NextViewType next_;

	GetElementFunc getElementFunc_;

};

template <class MatrixT>
class MatrixViewFunc {
public:

	using MatrixType = MatrixT;

	static const auto ROWS = MatrixType::ROWS;

	static const auto COLUMNS = MatrixType::COLUMNS;

	constexpr auto operator()(MatrixType& matrix, size_t row, size_t column) const noexcept
		-> decltype(auto)
	{
		return matrix[row][column];
	}

};

template <class MatrixType>
constexpr auto viewMatrix(MatrixType& matrix) noexcept {
	return MatrixView<MatrixType&, MatrixViewFunc<MatrixType>>(matrix);
}

template <class NextViewType>
class TransposedViewFunc {
public:

	using MatrixType = typename NextViewType::MatrixType;

	static const auto ROWS = NextViewType::ROWS;

	static const auto COLUMNS = NextViewType::COLUMNS;

	constexpr auto operator()(NextViewType nextView, size_t row, size_t column) const noexcept
		-> decltype(auto)
	{
		return nextView.get(column, row);
	}

};

template <class NextViewType>
constexpr auto transpose(NextViewType nextView) noexcept {
	return MatrixView<NextViewType, TransposedViewFunc<NextViewType>>(std::move(nextView));
}

template <class MatrixType>
constexpr auto viewMatrixTransposed(MatrixType& matrix) noexcept {
	return transpose(viewMatrix(matrix));
}

template <class NextViewType>
class SubmatrixViewFunc {
public:

	using MatrixType = typename NextViewType::MatrixType;

	static const auto ROWS = NextViewType::ROWS - 1;

	static const auto COLUMNS = NextViewType::COLUMNS - 1;

	constexpr SubmatrixViewFunc(size_t noRow, size_t noColumn) :
		noRow_(noRow),
		noColumn_(noColumn)
	{
		assert(noRow < NextViewType::ROWS);
		assert(noColumn < NextViewType::COLUMNS);
	}

	constexpr auto operator()(NextViewType nextView, size_t row, size_t column) const noexcept
		-> decltype(auto)
	{
		return nextView.get((row < noRow_) ? row : row + 1, (column < noColumn_) ? column : column + 1);
	}

private:

	size_t noRow_;

	size_t noColumn_;

};

template <class NextViewType>
constexpr auto submatrix(NextViewType nextView, size_t noRow, size_t noColumn) noexcept {
	return MatrixView<NextViewType, SubmatrixViewFunc<NextViewType>>(
		std::move(nextView), SubmatrixViewFunc<NextViewType>(noRow, noColumn));
}

template <class MatrixType>
constexpr auto viewSubmatrix(MatrixType& matrix, size_t noRow, size_t noColumn) noexcept {
	return submatrix(viewMatrix(matrix), noRow, noColumn);
}

template <
	class ScalarType,
	size_t ROWS_PARAM,
	size_t COLUMNS_PARAM,
	class ScalarEqualityFunc = ScalarEqual<ScalarType>
	>
class Matrix :
	boost::equality_comparable<Matrix<ScalarType, ROWS_PARAM, COLUMNS_PARAM, ScalarEqualityFunc>,
	boost::additive<Matrix<ScalarType, ROWS_PARAM, COLUMNS_PARAM, ScalarEqualityFunc>,
	boost::multiplicative<Matrix<ScalarType, ROWS_PARAM, COLUMNS_PARAM, ScalarEqualityFunc>, ScalarType
	>>>
{
public:

	using Scalar = ScalarType;

	static const auto ROWS = ROWS_PARAM;

	static const auto COLUMNS = COLUMNS_PARAM;

	static const Matrix IDENTITY;

	using Row = Vector<ScalarType, COLUMNS, ScalarEqualityFunc>;

	using Column = Vector<ScalarType, ROWS, ScalarEqualityFunc>;

	using ColumnReference = Vector<ScalarType&, COLUMNS, ScalarEqualityFunc>;

	// --- CONSTRUCTORS AND OPERATORS

	constexpr Matrix() noexcept = default;
	
	template <class NVT, class GEF>
	Matrix(const MatrixView<NVT, GEF>& view) {
		for (size_t rowIndex = 0; rowIndex < ROWS; ++rowIndex) {
			for (size_t columnIndex = 0; columnIndex < COLUMNS; ++columnIndex) {
				(*this)[rowIndex][columnIndex] = view.get(rowIndex, columnIndex);
			}
		}
	}

	// TODO: figure out a better constructor (this won't work in VS2015 either way)
	//template <class... CompatibleTypes>
	//explicit constexpr Matrix(CompatibleTypes&&... values) noexcept {
	//	static_assert(sizeof...(values) != ROWS * COLUMNS, "Bad number of arguments");
	//	elements_ = { std::forward<CompatibleTypes>(values)... };
	//}

	friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
		os << '<';
		std::copy(matrix.elements_.begin(), matrix.elements_.end(),
			coconut_tools::InfixOstreamIterator<Row>(os, ", "));
		os << '>';
		return os;
	}

	friend bool operator==(const Matrix& lhs, const Matrix& rhs) noexcept {
		return std::equal(lhs.elements_.begin(), lhs.elements_.end(), rhs.elements_.begin());
	}

	Matrix& operator+=(const Matrix& other) noexcept {
		std::transform(elements_.begin(), elements_.end(), other.elements_.begin(), elements_.begin(), std::plus<>());
		return *this;
	}

	Matrix& operator-=(const Matrix& other) noexcept {
		std::transform(elements_.begin(), elements_.end(), other.elements_.begin(), elements_.begin(), std::minus<>());
		return *this;
	}

	Matrix operator-() noexcept {
		auto result = Matrix();
		std::transform(elements_.begin(), elements_.end(), result.elements_.begin(), std::negate<>());
		return result;
	}

	template <class CompatibleMatrixType>
	friend std::enable_if_t<
		COLUMNS == CompatibleMatrixType::ROWS,
		Matrix<ScalarType, ROWS, CompatibleMatrixType::COLUMNS, ScalarEqualityFunc>
		> operator*(const Matrix& lhs, const CompatibleMatrixType& rhs) noexcept
	{
		auto result = Matrix<ScalarType, ROWS, CompatibleMatrixType::COLUMNS, ScalarEqualityFunc>();
		for (auto rowIndex = 0u; rowIndex < ROWS; ++rowIndex) {
			for (auto columnIndex = 0u; columnIndex < CompatibleMatrixType::COLUMNS; ++columnIndex) {
				result[rowIndex][columnIndex] = dot(lhs[rowIndex], rhs.column(columnIndex));
			}
		}
		return result;
	}

	template <class CompatibleMatrixType>
	std::enable_if_t<
		COLUMNS == ROWS &&
		CompatibleMatrixType::COLUMNS == CompatibleMatrixType::ROWS &&
		ROWS == CompatibleMatrixType::ROWS,
		Matrix&
		> operator*=(const CompatibleMatrixType& rhs) noexcept
	{
		const auto result = *this * rhs;
		*this = result;
		return *this;
	}

	Matrix& operator*=(Scalar scalar) noexcept {
		std::transform(elements_.begin(), elements_.end(), elements_.begin(), [scalar](auto element) {
			return element * scalar;
		});
		return *this;
	}

	friend Vector<Scalar, ROWS, ScalarEqualityFunc> operator*(
		const Matrix& matrix,
		const Vector<Scalar, ROWS, ScalarEqualityFunc>& vector
		) noexcept
	{
		auto result = Vector<Scalar, ROWS, ScalarEqualityFunc>();
		for (auto columnIndex = 0u; columnIndex < COLUMNS; ++columnIndex) {
			result[columnIndex] = dot(matrix.column(columnIndex), vector);
		}
		return result;
	}

	Matrix& operator/=(Scalar scalar) noexcept {
		std::transform(elements_.begin(), elements_.end(), elements_.begin(), [scalar](auto element) {
			return element / scalar;
		});
		return *this;
	}

	// --- MATRIX-SPECIFIC OPERATIONS

	constexpr Matrix<Scalar, COLUMNS, ROWS, ScalarEqualityFunc> transpose() const noexcept {
		return viewMatrixTransposed(*this);
	}

	Scalar determinant() const noexcept {
		return viewMatrix(*this).determinant();
	}

	Scalar cofactor(size_t rowIndex, size_t columnIndex) const noexcept {
		return viewMatrix(*this).cofactor(rowIndex, columnIndex);
	}

	Matrix inverse() const noexcept {
		return viewMatrix(*this).inverse();
	}

	// --- ACCESSORS

	constexpr const Row& operator[](size_t rowIndex) const noexcept {
		assert(rowIndex < ROWS);
		return elements_[rowIndex];
	}

	Row& operator[](size_t rowIndex) noexcept {
		assert(rowIndex < ROWS);
		return elements_[rowIndex];
	}

	template <size_t ROW, size_t COLUMN>
	constexpr std::enable_if_t<(ROW < ROWS && COLUMN < COLUMNS), const Scalar&> get() const noexcept {
		return elements_[ROW].get<COLUMN>();
	}

	template <size_t ROW, size_t COLUMN>
	std::enable_if_t<(ROW < ROWS && COLUMN < COLUMNS), Scalar&> get() noexcept {
		return elements_[ROW].get<COLUMN>();
	}

	constexpr const Row& row(size_t rowIndex) const noexcept {
		assert(rowIndex < ROWS);
		return elements_[rowIndex];
	}

	Row& row(size_t rowIndex) noexcept {
		assert(rowIndex < ROWS);
		return elements_[rowIndex];
	}

	Column column(size_t columnIndex) const noexcept {
		assert(columnIndex < COLUMNS);
		auto column = Column();
		getColumn_<>(column, columnIndex);
		return column;
	}

	auto view() const noexcept {
		return viewMatrix(*this);
	}

	auto view() noexcept {
		return viewMatrix(*this);
	}

private:

	std::array<Row, ROWS> elements_;

	template <size_t ROW = 0>
	void getColumn_(Column& column, size_t columnIndex) const {
		column.get<ROW>() = elements_[ROW][columnIndex];
		getColumn_<ROW + 1>(column, columnIndex);
	}

	template <>
	void getColumn_<ROWS>(Column&, size_t) const {
	}

};

using Matrix4x4 = Matrix<float, 4, 4>;
static_assert(sizeof(Matrix4x4) == sizeof(float) * 16, "Empty base optimisation didn't work");
static_assert(std::is_trivially_copyable<Matrix4x4>::value, "Matrix is not trivially copyable");

namespace detail {

template <class ST, size_t R, size_t C, class SEF>
Matrix<ST, R, C, SEF> makeIdentity() {
	auto identity = Matrix<ST, R, C, SEF>();
	for (auto row = 0u; row < R; ++row) {
		identity[row][row] = ST(1);
	}
	return identity;
}

} // namespace detail

template <class ST, size_t R, size_t C, class SEF>
const Matrix<ST, R, C, SEF>	Matrix<ST, R, C, SEF>::IDENTITY =
	detail::makeIdentity<ST, R, C, SEF>();

} // namespace math

using math::Matrix;
using math::Matrix4x4;

} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_MATH_MATRIX_HPP_ */
