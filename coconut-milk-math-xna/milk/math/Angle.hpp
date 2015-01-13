#ifndef _COCONUT_MILK_MATH_ANGLE_HPP_
#define _COCONUT_MILK_MATH_ANGLE_HPP_

namespace coconut {
namespace milk {
namespace math {

struct Radians {

	float angle;

	explicit Radians(float angle) :
		angle(angle)
	{
	}

};

const Radians PI(
	static_cast<float>(3.14159265358979323846264338327950288419716939937510582097494459230781640628620899));

struct Degrees {

	float angle;

	explicit Degrees(float angle) :
		angle(angle)
	{
	}

};

class Angle {
public:

	static const Angle RIGHT;

	static const Angle HALF_FULL;

	static const Angle FULL;

	Angle(Radians radians) :
		radians_(radians.angle)
	{
	}

	Angle(Degrees degrees) :
		radians_(degrees)
	{
	}

	float radians() const {
		return radians_;
	}

	float degrees() const {
		return radians_;
	}

private:

	float radians_;

};

} // namespace math
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_MATH_ANGLE_HPP_ */
