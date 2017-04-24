#ifndef _COCONUT_PULP_MATH_VECTOR_HPP_
#define _COCONUT_PULP_MATH_VECTOR_HPP_

#include <cmath>
#include <cassert>
#include <array>
#include <algorithm>
#include <functional>
#include <memory>
#include <numeric>
#include <iosfwd>
#include <initializer_list>
#include <type_traits>

#include <boost/operators.hpp>

#include "coconut-tools/utils/InfixOstreamIterator.hpp"

#include "ScalarEqual.hpp"

namespace coconut {
namespace pulp {
namespace math {

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

	template <class... CompatibleTypes>
	explicit constexpr Vector(CompatibleTypes&&... values) noexcept {
		static_assert(sizeof...(values) <= DIMENSIONS, "Too many values");
		elements_ = { std::forward<CompatibleTypes>(values)... };
		std::uninitialized_fill(
			elements_.begin() + sizeof...(values),
			elements_.end(),
			Scalar(0)
			);
	}

	constexpr Vector(std::initializer_list<Scalar> values) noexcept {
		assert(values.size() <= DIMENSIONS);
		std::copy(values.begin(), values.end(), elements_.begin());
		std::uninitialized_fill(
			elements_.begin() + values.size(),
			elements_.end(),
			Scalar(0)
			);
	}

	friend std::ostream& operator<<(std::ostream& os, const Vector& vector) {
		os << '<';
		std::copy(vector.elements_.begin(), vector.elements_.end(),
			coconut_tools::InfixOstreamIterator<ScalarType>(os, ", "));
		os << '>';
		return os;
	}

	friend bool operator==(const Vector& lhs, const Vector& rhs) noexcept {
		return std::equal(lhs.elements_.begin(), lhs.elements_.end(), rhs.elements_.begin(), ScalarEqualityFunc());
	}

	Vector& operator+=(const Vector& other) noexcept {
		std::transform(elements_.begin(), elements_.end(), other.elements_.begin(), elements_.begin(), std::plus<>());
		return *this;
	}

	Vector& operator-=(const Vector& other) noexcept {
		std::transform(elements_.begin(), elements_.end(), other.elements_.begin(), elements_.begin(), std::minus<>());
		return *this;
	}

	Vector operator-() const noexcept {
		auto result = Vector();
		std::transform(elements_.begin(), elements_.end(), result.elements_.begin(), std::negate<>());
		return result;
	}

	Vector& operator*=(Scalar scalar) noexcept {
		std::transform(elements_.begin(), elements_.end(), elements_.begin(), [scalar](auto element) {
				return element * scalar;
			});
		return *this;
	}

	Vector& operator/=(Scalar scalar) noexcept {
		std::transform(elements_.begin(), elements_.end(), elements_.begin(), [scalar](auto element) {
			return element / scalar;
		});
		return *this;
	}

	// --- CONVERTERS

	template <size_t DIMENSIONS_PARAM_ = DIMENSIONS_PARAM>
	std::enable_if_t<(DIMENSIONS_PARAM_> 2), Vector<Scalar, 3, ScalarEqualityFunc>>
		xyz() const noexcept
	{
		static_assert(DIMENSIONS_PARAM_ == DIMENSIONS_PARAM, "Dimensions changed");
		return Vector<Scalar, 3, ScalarEqualityFunc>(x(), y(), z());
	}

	// --- VECTOR-SPECIFIC OPERATIONS

	friend Scalar dot(const Vector& lhs, const Vector& rhs) noexcept {
		return std::inner_product(lhs.elements_.begin(), lhs.elements_.end(), rhs.elements_.begin(), Scalar(0));
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
		if (lengthSq() > Scalar(0)) {
			*this /= length();
		}
		return *this;
	}

	Vector normalised() const noexcept {
		auto result = *this;
		return result.normalise();
	}

	// --- ACCESSORS

	constexpr const Scalar& operator[](size_t index) const noexcept {
		assert(index < DIMENSIONS);
		return elements_[index];
	}

	Scalar& operator[](size_t index) noexcept {
		assert(index < DIMENSIONS);
		return elements_[index];
	}

	template <size_t INDEX>
	constexpr std::enable_if_t<(DIMENSIONS > INDEX), const Scalar&> get() const noexcept {
		return elements_[INDEX];
	}

	template <size_t INDEX>
	std::enable_if_t<(DIMENSIONS > INDEX), Scalar&> get() noexcept {
		return elements_[INDEX];
	}

	template <size_t DIMENSIONS_PARAM_ = DIMENSIONS_PARAM>
	constexpr std::enable_if_t<(DIMENSIONS_PARAM_> 0), const Scalar&> x() const noexcept {
		static_assert(DIMENSIONS_PARAM_ == DIMENSIONS_PARAM, "Dimensions changed");
		return get<0>();
	}

	template <size_t DIMENSIONS_PARAM_ = DIMENSIONS_PARAM>
	std::enable_if_t<(DIMENSIONS_PARAM_> 0), Scalar&> x() noexcept {
		static_assert(DIMENSIONS_PARAM_ == DIMENSIONS_PARAM, "Dimensions changed");
		return get<0>();
	}

	template <size_t DIMENSIONS_PARAM_ = DIMENSIONS_PARAM>
	constexpr std::enable_if_t<(DIMENSIONS_PARAM_> 1), const Scalar&> y() const noexcept {
		static_assert(DIMENSIONS_PARAM_ == DIMENSIONS_PARAM, "Dimensions changed");
		return get<1>();
	}

	template <size_t DIMENSIONS_PARAM_ = DIMENSIONS_PARAM>
	std::enable_if_t<(DIMENSIONS_PARAM_> 1), Scalar&> y() noexcept {
		static_assert(DIMENSIONS_PARAM_ == DIMENSIONS_PARAM, "Dimensions changed");
		return get<1>();
	}

	template <size_t DIMENSIONS_PARAM_ = DIMENSIONS_PARAM>
	constexpr std::enable_if_t<(DIMENSIONS_PARAM_ > 2), const Scalar&> z() const noexcept {
		static_assert(DIMENSIONS_PARAM_ == DIMENSIONS_PARAM, "Dimensions changed");
		return get<2>();
	}

	template <size_t DIMENSIONS_PARAM_ = DIMENSIONS_PARAM>
	std::enable_if_t<(DIMENSIONS_PARAM_> 2), Scalar&> z() noexcept {
		static_assert(DIMENSIONS_PARAM_ == DIMENSIONS_PARAM, "Dimensions changed");
		return get<2>();
	}

	template <size_t DIMENSIONS_PARAM_ = DIMENSIONS_PARAM>
	constexpr std::enable_if_t<(DIMENSIONS_PARAM_> 3), const Scalar&> w() const noexcept {
		static_assert(DIMENSIONS_PARAM_ == DIMENSIONS_PARAM, "Dimensions changed");
		return get<3>();
	}

	template <size_t DIMENSIONS_PARAM_ = DIMENSIONS_PARAM>
	std::enable_if_t<(DIMENSIONS_PARAM_> 3), Scalar&> w() noexcept {
		static_assert(DIMENSIONS_PARAM_ == DIMENSIONS_PARAM, "Dimensions changed");
		return get<3>();
	}

private:

	std::array<Scalar, DIMENSIONS> elements_;

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
static_assert(std::is_trivially_copyable<Vec2>::value, "Vector is not trivially copiable");

using Vec3 = Vector<float, 3>;
static_assert(sizeof(Vec3) == sizeof(float) * 3, "Empty base optimisation didn't work");
static_assert(std::is_trivially_copyable<Vec3>::value, "Vector is not trivially copiable");

using Vec4 = Vector<float, 4>;
static_assert(sizeof(Vec4) == sizeof(float) * 4, "Empty base optimisation didn't work");
static_assert(std::is_trivially_copyable<Vec4>::value, "Vector is not trivially copiable");

} // namespace math

using math::Vec2;
using math::Vec3;
using math::Vec4;

} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_MATH_VECTOR_HPP_ */
