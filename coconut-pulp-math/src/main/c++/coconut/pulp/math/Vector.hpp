#ifndef _COCONUT_PULP_MATH_VECTOR_HPP_
#define _COCONUT_PULP_MATH_VECTOR_HPP_

#include <cmath>
#include <cassert>
#include <array>
#include <algorithm>
#include <functional>
#include <iterator>
#include <numeric>
#include <iosfwd>
#include <initializer_list>

#include <type_traits>

#include <boost/operators.hpp>

#include "coconut-tools/utils/InfixOstreamIterator.hpp"

namespace coconut {
namespace pulp {
namespace math {

template <class ScalarType>
struct ScalarEqual {

	constexpr bool operator()(ScalarType lhs, ScalarType rhs) const noexcept {
		return std::equal_to<ScalarType>()(lhs, rhs);
	}

};

template <>
struct ScalarEqual<float> {

	constexpr bool operator()(float lhs, float rhs) const noexcept {
		return std::abs(lhs - rhs) < (1.0f / 10000.0f);
	}

};

template <class ScalarType, size_t DIMENSIONS_PARAM, class ScalarEqualityFunc = ScalarEqual<ScalarType>>
class Vector :
	boost::equality_comparable<Vector<ScalarType, DIMENSIONS_PARAM, ScalarEqualityFunc>,
	boost::additive<Vector<ScalarType, DIMENSIONS_PARAM, ScalarEqualityFunc>,
	boost::multiplicative<Vector<ScalarType, DIMENSIONS_PARAM, ScalarEqualityFunc>, ScalarType
	>>>
{
public:

	using Scalar = ScalarType;

	static const auto DIMENSIONS = DIMENSIONS_PARAM;

	// --- CONSTRUCTORS AND OPERATORS

	// TODO: uncomment after switching to VS2017. VS2015 has a bug that
	// confuses this default constructor with the variadic template one.
	// constexpr Vector() noexcept = default;

	template <class... CompatibleTypes>
	explicit constexpr Vector(CompatibleTypes&&... values) noexcept {
		static_assert(sizeof...(values) <= DIMENSIONS, "Too many values");
		data_ = { std::forward<CompatibleTypes>(values)... };
	}

	friend std::ostream& operator<<(std::ostream& os, const Vector& vector) {
		os << '<';
		std::copy(vector.data_.begin(), vector.data_.end(),
			coconut_tools::InfixOstreamIterator<ScalarType>(os, ", "));
		os << '>';
		return os;
	}

	friend bool operator==(const Vector& lhs, const Vector& rhs) noexcept {
		return std::equal(lhs.data_.begin(), lhs.data_.end(), rhs.data_.begin(), ScalarEqualityFunc());
	}

	Vector& operator+=(const Vector& other) noexcept {
		std::transform(data_.begin(), data_.end(), other.data_.begin(), data_.begin(), std::plus<>());
		return *this;
	}

	Vector& operator-=(const Vector& other) noexcept {
		std::transform(data_.begin(), data_.end(), other.data_.begin(), data_.begin(), std::minus<>());
		return *this;
	}

	friend Vector operator-(const Vector& vector) noexcept {
		auto result = Vector();
		std::transform(vector.data_.begin(), vector.data_.end(), result.data_.begin(), std::negate<>());
		return result;
	}

	Vector& operator*=(Scalar scalar) noexcept {
		std::transform(data_.begin(), data_.end(), data_.begin(), [scalar](auto element) {
				return element * scalar;
			});
		return *this;
	}

	Vector& operator/=(Scalar scalar) noexcept {
		std::transform(data_.begin(), data_.end(), data_.begin(), [scalar](auto element) {
			return element / scalar;
		});
		return *this;
	}

	// --- VECTOR-SPECIFIC OPERATIONS

	friend Scalar dot(const Vector& lhs, const Vector& rhs) noexcept {
		return std::inner_product(lhs.data_.begin(), lhs.data_.end(), rhs.data_.begin(), Scalar(0));
	}

	Vector& crossEq(const Vector& other) noexcept {
		*this = cross(*this, other);
		return *this;
	}

	Scalar length() const noexcept {
		using std::sqrt;
		return sqrt(dot(*this, *this));
	}

	Scalar lengthSq() const noexcept {
		using std::sqrt;
		return dot(*this, *this);
	}

	Vector& normalise() noexcept {
		assert(length() > 0.0f);
		*this /= length();
		return *this;
	}

	Vector normalised() const noexcept {
		auto result = *this;
		return result.normalise();
	}

	// --- ACCESSORS

	template <size_t INDEX>
	constexpr std::enable_if_t<(DIMENSIONS > INDEX), const Scalar&> get() const noexcept {
		return data_[INDEX];
	}

	template <size_t INDEX>
	std::enable_if_t<(DIMENSIONS > INDEX), Scalar&> get() noexcept {
		return data_[INDEX];
	}

	template <size_t DIMENSIONS_PARAM_ = DIMENSIONS_PARAM>
	constexpr std::enable_if_t<(DIMENSIONS_PARAM_> 0), const Scalar&> x() const noexcept {
		return get<0>();
	}

	template <size_t DIMENSIONS_PARAM_ = DIMENSIONS_PARAM>
	std::enable_if_t<(DIMENSIONS_PARAM_> 0), Scalar&> x() noexcept {
		return get<0>();
	}

	template <size_t DIMENSIONS_PARAM_ = DIMENSIONS_PARAM>
	constexpr std::enable_if_t<(DIMENSIONS_PARAM_> 1), const Scalar&> y() const noexcept {
		return get<1>();
	}

	template <size_t DIMENSIONS_PARAM_ = DIMENSIONS_PARAM>
	std::enable_if_t<(DIMENSIONS_PARAM_> 1), Scalar&> y() noexcept {
		return get<1>();
	}

	template <size_t DIMENSIONS_PARAM_ = DIMENSIONS_PARAM>
	constexpr std::enable_if_t<(DIMENSIONS_PARAM_ > 2), const Scalar&> z() const noexcept {
		return get<2>();
	}

	template <size_t DIMENSIONS_PARAM_ = DIMENSIONS_PARAM>
	std::enable_if_t<(DIMENSIONS_PARAM_> 2), Scalar&> z() noexcept {
		return get<2>();
	}

	template <size_t DIMENSIONS_PARAM_ = DIMENSIONS_PARAM>
	constexpr std::enable_if_t<(DIMENSIONS_PARAM_> 3), const Scalar&> w() const noexcept {
		return get<3>();
	}

	template <size_t DIMENSIONS_PARAM_ = DIMENSIONS_PARAM>
	std::enable_if_t<(DIMENSIONS_PARAM_> 3), Scalar&> w() noexcept {
		return get<3>();
	}

private:

	std::array<Scalar, DIMENSIONS> data_;

};

template <class VectorType>
constexpr std::enable_if_t<VectorType::DIMENSIONS == 3, VectorType>
	cross(const VectorType& lhs, const VectorType& rhs) noexcept
{
	return VectorType{
		(lhs.y() * rhs.z()) - (lhs.z() * rhs.y()),
		(lhs.z() * rhs.x()) - (lhs.x() * rhs.z()),
		(lhs.x() * rhs.y()) - (lhs.y() * rhs.x())
	};
}

using Vec2 = Vector<float, 2>;
static_assert(sizeof(Vec2) == sizeof(float) * 2, "Empty base optimisation didn't work");
// TODO: uncomment these after switching to VS2017
//static_assert(std::is_trivial<Vec2>::value, "Vector is not trivial");

using Vec3 = Vector<float, 3>;
static_assert(sizeof(Vec3) == sizeof(float) * 3, "Empty base optimisation didn't work");
//static_assert(std::is_trivial<Vec3>::value, "Vector is not trivial");

using Vec4 = Vector<float, 4>;
static_assert(sizeof(Vec4) == sizeof(float) * 4, "Empty base optimisation didn't work");
//static_assert(std::is_trivial<Vec4>::value, "Vector is not trivial");

} // namespace math

using math::Vec2;
using math::Vec3;
using math::Vec4;

} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_MATH_VECTOR_HPP_ */
