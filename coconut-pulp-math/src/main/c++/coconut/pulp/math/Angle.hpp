#ifndef _COCONUT_PULP_MATH_ANGLE_HPP_
#define _COCONUT_PULP_MATH_ANGLE_HPP_

#include <iosfwd>

#include <boost/operators.hpp>

namespace coconut {
namespace pulp {
namespace math {

const float PI = static_cast<float>(3.14159264f);

class Angle :
	boost::less_than_comparable<Angle,
	boost::equality_comparable<Angle,
	boost::additive<Angle,
	boost::multiplicative<Angle, float>>>>
{
public:

	static const Angle RIGHT;

	static const Angle HALF_FULL;

	static const Angle FULL;

	constexpr float radians() const noexcept {
		return radians_;
	}

	constexpr float degrees() const noexcept {
		return radians_ * (180.0f / PI);
	}

	constexpr bool operator==(const Angle& rhs) const noexcept {
		return radians_ == rhs.radians_;
	}

	constexpr bool operator<(const Angle& rhs) const noexcept {
		return radians_ < rhs.radians_;
	}

	Angle& operator+=(const Angle& rhs) noexcept {
		radians_ += rhs.radians_;
		return *this;
	}

	Angle& operator-=(const Angle& rhs) noexcept {
		radians_ -= rhs.radians_;
		return *this;
	}

	Angle& operator*=(float rhs) noexcept {
		radians_ *= rhs;
		return *this;
	}

	Angle& operator/=(float rhs) noexcept {
		radians_ /= rhs;
		return *this;
	}

	Angle operator-() const noexcept {
		return -1.0f * (*this);
	}

	friend const Angle radians(float radians) noexcept;

	friend const Angle degrees(float degrees) noexcept;

private:

	constexpr explicit Angle(float radians) noexcept :
		radians_(radians)
	{
	}

private:

	float radians_;

};

static_assert(sizeof(Angle) == sizeof(float), "Angle should have no extra data");

std::ostream& operator<<(std::ostream& os, const Angle& angle);

inline const Angle radians(float radians) noexcept {
	return Angle(radians);
}

inline const Angle degrees(float degrees) noexcept {
	return Angle(degrees * (PI / 180.0f));
}

inline const Angle operator""_rad(long double r) noexcept {
	return radians(r);
}

inline const Angle operator""_deg(long double d) noexcept {
	return degrees(d);
}

} // namespace math

using math::Angle;
using math::radians;
using math::degrees;
using math::operator ""_deg;
using math::operator ""_rad;

namespace math_literals {

using math::operator ""_deg;
using math::operator ""_rad;

} // namespace math_literals

} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_MATH_ANGLE_HPP_ */
