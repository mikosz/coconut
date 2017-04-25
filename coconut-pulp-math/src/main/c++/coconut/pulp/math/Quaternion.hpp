#ifndef _COCONUT_PULP_MATH_QUATERNION_HPP_
#define _COCONUT_PULP_MATH_QUATERNION_HPP_

#include <boost/operators.hpp>

#include "ScalarEqual.hpp"
#include "Vector.hpp"

namespace coconut {
namespace pulp {
namespace math {

template <class ScalarType, class ScalarEqualityFunc = ScalarEqual<ScalarType>>
class Quaternion :
	boost::equality_comparable<Quaternion<ScalarType, ScalarEqualityFunc>,
	boost::additive<Quaternion<ScalarType, ScalarEqualityFunc>,
	boost::multipliable<Quaternion<ScalarType, ScalarEqualityFunc>,
	boost::multiplicative<Quaternion<ScalarType, ScalarEqualityFunc>, ScalarType
	>>>>
{
public:

	using Scalar = ScalarType;

	using ScalarPart = Scalar;

	using VectorPart = Vector<Scalar, 3, ScalarEqualityFunc>;

	// --- CONSTRUCTORS AND OPERATORS

	constexpr Quaternion(ScalarPart s, VectorPart v) :
		elements_(v.x(), v.y(), v.z(), s)
	{
	}

	friend std::ostream& operator<<(std::ostream& os, const Quaternion& q) {
		return os << q.s() << " + " << q.v();
	}

	friend bool operator==(const Quaternion& lhs, const Quaternion& rhs) noexcept {
		return lhs.elements_ == rhs.elements_;
	}

	Quaternion& operator*=(const Quaternion& other) noexcept {
		const auto s1 = s();
		const auto s2 = other.s();
		const auto v1 = v();
		const auto v2 = other.v();

		const auto s = s1 * s2 - dot(v1, v2);
		const auto v = s1 * v2 + s2 * v1 + cross(v1, v2);

		*this = Quaternion(s, v);

		return *this;
	}

	Quaternion& operator+=(const Quaternion& other) noexcept {
		elements_ += other.elements_;
		return *this;
	}

	Quaternion& operator-=(const Quaternion& other) noexcept {
		elements_ -= other.elements_;
		return *this;
	}

	Quaternion& operator*=(const Scalar& s) noexcept {
		elements_ *= s;
		return *this;
	}

	Quaternion& operator/=(const Scalar& s) noexcept {
		elements_ /= s;
		return *this;
	}

	// --- QUATERNION-SPECIFIC OPERATIONS

	constexpr Quaternion conjugate() const noexcept {
		return Quaternion(s(), -v());
	}

	Quaternion& normalise() noexcept {
		const auto n = norm();
		if (n > Scalar(0)) {
			*this /= n;
		}
		return *this;
	}

	Quaternion normalised() const noexcept {
		auto result = *this;
		return result.normalise();
	}

	Scalar norm() const noexcept {
		return elements_.length();
	}

	Scalar normSq() const noexcept {
		return elements_.lengthSq();
	}

	Quaternion inverse() const noexcept {
		const auto n = normSq();
		assert(!ScalarEqualityFunc()(n, Scalar(0)));
		return conjugate() / n;
	}

	// --- ACCESSORS

	constexpr const ScalarPart s() const noexcept {
		return elements_.w();
	}

	ScalarPart& s() noexcept {
		return elements_.w();
	}

	const VectorPart v() const noexcept { // TODO: return view
		return elements_.xyz();
	}

	const Scalar x() const noexcept {
		return elements_.x();
	}

	Scalar& x() noexcept {
		return elements_.x();
	}

	const Scalar y() const noexcept {
		return elements_.y();
	}

	Scalar& y() noexcept {
		return elements_.y();
	}

	const Scalar z() const noexcept {
		return elements_.z();
	}

	Scalar& z() noexcept {
		return elements_.z();
	}

	const Scalar w() const noexcept {
		return elements_.w();
	}

	Scalar& w() noexcept {
		return elements_.w();
	}

private:

	using Elements = Vector<Scalar, 4, ScalarEqualityFunc>;
	
	Elements elements_;

};

using Quat = Quaternion<float>;

} // namespace math

using math::Quaternion;
using math::Quat;

} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_MATH_QUATERNION_HPP_ */
