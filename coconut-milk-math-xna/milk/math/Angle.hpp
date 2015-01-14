#ifndef _COCONUT_MILK_MATH_ANGLE_HPP_
#define _COCONUT_MILK_MATH_ANGLE_HPP_

#include <iosfwd>

#include <boost/operators.hpp>

namespace coconut {
namespace milk {
namespace math {

const float PI = static_cast<float>(3.14159265358979323846264338327950288419716939937510582097494459230781640628620899);

class Angle :
	boost::less_than_comparable<Angle>,
	boost::equality_comparable<Angle>,
	boost::additive<Angle>,
	boost::multiplicative<Angle, float>
{
public:

	static const Angle RIGHT;

	static const Angle HALF_FULL;

	static const Angle FULL;

	float radians() const {
		return radians_;
	}

	float degrees() const;

	bool operator==(const Angle& rhs) const {
		return radians_ == rhs.radians_;
	}

	bool operator<(const Angle& rhs) const {
		return radians_ < rhs.radians_;
	}

	Angle& operator+=(const Angle& rhs) {
		radians_ += rhs.radians_;
		return *this;
	}

	Angle& operator-=(const Angle& rhs) {
		radians_ -= rhs.radians_;
		return *this;
	}

	Angle& operator*=(float rhs) {
		radians_ *= rhs;
		return *this;
	}

	Angle& operator/=(float rhs) {
		radians_ /= rhs;
		return *this;
	}

	Angle operator-() const {
		return -1.0f * (*this);
	}

	friend Angle radians(float radians);

	friend Angle degrees(float degrees);

private:

	explicit Angle(float radians) :
		radians_(radians)
	{
	}

private:

	float radians_;

};

std::ostream& operator<<(std::ostream& os, const Angle& angle);

Angle radians(float radians);

Angle degrees(float degrees);

} // namespace math
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_MATH_ANGLE_HPP_ */
