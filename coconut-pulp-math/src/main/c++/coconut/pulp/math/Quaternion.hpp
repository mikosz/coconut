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
	boost::multipliable<Quaternion<ScalarType, ScalarEqualityFunc>
	>>
{
public:

	using Scalar = ScalarType;

	using ScalarPart = Scalar;

	using VectorPart = Vector<Scalar, 3, ScalarEqualityFunc>;

	// --- CONSTRUCTORS AND OPERATORS

	constexpr Quaternion(ScalarPart s, VectorPart v) :
		s_(std::move(s)),
		v_(std::move(v))
	{
	}

	friend std::ostream& operator<<(std::ostream& os, const Quaternion& q) {
		return os << q.s_ << " + " << q.v_;
	}

	friend bool operator==(const Quaternion& lhs, const Quaternion& rhs) noexcept {
		return ScalarEqualityFunc()(lhs.s_, rhs.s_) && lhs.v_ == rhs.v_;
	}

	Quaternion& operator*=(const Quaternion& other) noexcept {
		const auto s = s_ * other.s_ - dot(v_, other.v_);
		const auto v = s_ * other.v_ + other.s_ * v_ + cross(v_, other.v_);

		s_ = std::move(s);
		v_ = std::move(v);

		return *this;
	}

	constexpr const ScalarPart s() const noexcept {
		return s_;
	}

	ScalarPart& s() noexcept {
		return s_;
	}

	constexpr const VectorPart& v() const noexcept {
		return v_;
	}

	VectorPart& v() noexcept {
		return v_;
	}

	const Scalar x() const noexcept {
		return v_.x();
	}

	Scalar& x() noexcept {
		return v_.x();
	}

	const Scalar y() const noexcept {
		return v_.y();
	}

	Scalar& y() noexcept {
		return v_.y();
	}

	const Scalar z() const noexcept {
		return v_.z();
	}

	Scalar& z() noexcept {
		return v_.z();
	}

	const Scalar w() const noexcept {
		return s_;
	}

	Scalar& w() noexcept {
		return s_;
	}

private:

	ScalarPart s_;
	
	VectorPart v_;

};

using Quat = Quaternion<float>;

} // namespace math

using math::Quaternion;
using math::Quat;

} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_MATH_QUATERNION_HPP_ */
