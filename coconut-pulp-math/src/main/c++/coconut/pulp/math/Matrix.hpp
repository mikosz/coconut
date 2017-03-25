#ifndef _COCONUT_PULP_MATH_MATRIX_HPP_
#define _COCONUT_PULP_MATH_MATRIX_HPP_

#include <array>
#include <type_traits>
#include <iosfwd>
#include <algorithm>
#include <functional>

#include <boost/operators.hpp>

#include "coconut-tools/utils/InfixOstreamIterator.hpp"

#include "Handedness.hpp"
#include "Vector.hpp"

namespace coconut {
namespace pulp {
namespace math {

template <
	class ScalarType,
	size_t ROWS_PARAM,
	size_t COLUMNS_PARAM,
	class ScalarEqualityFunc = ScalarEqual<ScalarType>
	>
class Matrix :
	boost::equality_comparable<Matrix<ScalarType, ROWS_PARAM, COLUMNS_PARAM, ScalarEqualityFunc>,
	boost::additive<Matrix<ScalarType, ROWS_PARAM, COLUMNS_PARAM, ScalarEqualityFunc>,
	boost::multipliable<Matrix<ScalarType, COLUMNS_PARAM, ROWS_PARAM, ScalarEqualityFunc>,
	boost::multiplicative<Matrix<ScalarType, ROWS_PARAM, COLUMNS_PARAM, ScalarEqualityFunc>, ScalarType
	>>>>
{
public:

	using Scalar = ScalarType;

	static const auto ROWS = ROWS_PARAM;

	static const auto COLUMNS = COLUMNS_PARAM;

	static const Matrix IDENTITY;

	using Row = Vector<ScalarType, ROWS, ScalarEqualityFunc>;

	using Column = Vector<ScalarType, COLUMNS, ScalarEqualityFunc>;

	using ColumnReference = Vector<ScalarType&, COLUMNS, ScalarEqualityFunc>;

	// --- CONSTRUCTORS AND OPERATORS

	constexpr Matrix() noexcept = default;
	
	// TODO: uncomment after switching to VS2017. VS2015 has a bug that
	// confuses this default constructor with the variadic template one.
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
		getColumn<>(column, columnIndex);
		return column;
	}

private:

	std::array<Row, ROWS> elements_;

	template <size_t ROW = 0>
	void getColumn(Column& column, size_t columnIndex) const {
		column.get<ROW>() = elements_[ROW][columnIndex];
		getColumn<ROW + 1>(column, columnIndex);
	}

	template <>
	void getColumn<ROWS>(Column&, size_t) const {
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

using math::Matrix4x4;

} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_MATH_MATRIX_HPP_ */


#ifndef _COCONUT_PULP_MATH_MATRIX_HPP_
#define _COCONUT_PULP_MATH_MATRIX_HPP_

#include <stdexcept>

#include <DirectXMath.h>

#include "Handedness.hpp"
#include "Vector.hpp"

namespace coconut {
namespace pulp {
namespace math {

#pragma message("!!!!!VERY VERY TEMP!")
// TODO: TEMP Replace with portable implementation
class Matrix {
public:

	static const Matrix IDENTITY;

	static Matrix translation(const Vec3& translationVector);

	static Matrix translation(float x, float y, float z) {
		return DirectX::XMMatrixTranslation(x, y, z);
	}

	static Matrix scale(const Vec3& scaleVector);

	static Matrix scale(float x, float y, float z) {
		return DirectX::XMMatrixScaling(x, y, z);
	}

	static Matrix rotation(const Vec3& rotationVector);

	static Matrix rotation(float x, float y, float z) {
		return DirectX::XMMatrixRotationRollPitchYaw(x, y, z);
	}

	static Matrix orthographicProjection(
		Handedness handedness, float width, float height, float nearZ, float farZ) {
		switch (handedness) {
		case Handedness::LEFT:
			return DirectX::XMMatrixOrthographicLH(width, height, nearZ, farZ);
		case Handedness::RIGHT:
			return DirectX::XMMatrixOrthographicRH(width, height, nearZ, farZ);
		default:
			throw std::logic_error("Unknown handedness value");
		}
	}

	// TODO: change float fov to a Angle type
	static Matrix perspectiveProjection(
		Handedness handedness, float fov, float aspectRatio, float nearZ, float farZ) {
		switch (handedness) {
		case Handedness::LEFT:
			return DirectX::XMMatrixPerspectiveFovLH(fov, aspectRatio, nearZ, farZ);
		case Handedness::RIGHT:
			return DirectX::XMMatrixPerspectiveFovRH(fov, aspectRatio, nearZ, farZ);
		default:
			throw std::logic_error("Unknown handedness value");
		}
	}

	Matrix() {
	}

	Matrix(const DirectX::XMMATRIX& xmMatrix) {
		DirectX::XMStoreFloat4x4(&internal_, xmMatrix);
	}

	Matrix operator *(const Matrix& other) { // TODO: use boost operator
		return DirectX::XMMatrixMultiply(
			DirectX::XMLoadFloat4x4(&internal_),
			DirectX::XMLoadFloat4x4(&other.internal_)
			);
	}

	Matrix& operator *=(const Matrix& other) {
		DirectX::XMStoreFloat4x4(
			&internal_,
			DirectX::XMMatrixMultiply(
				DirectX::XMLoadFloat4x4(&internal_),
				DirectX::XMLoadFloat4x4(&other.internal_)
				)
			);
		return *this;
	}

	Matrix transposed() const { // TODO: cache?
		return DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&internal_));
	}

	Matrix inverted() const { // TODO: cache?
		return DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&internal_));
	}

	Vec3 extractTranslation() const { // TODO: cache?
		return Vec3(internal_._14, internal_._24, internal_._34);
	}

	bool isIdentity() const {
		return DirectX::XMMatrixIsIdentity(DirectX::XMLoadFloat4x4(&internal_));
	}

	DirectX::XMMATRIX internal() const {
		return DirectX::XMLoadFloat4x4(&internal_);
	}

private:

	DirectX::XMFLOAT4X4 internal_;

};

} // namespace math
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_MATH_MATRIX_HPP_ */
