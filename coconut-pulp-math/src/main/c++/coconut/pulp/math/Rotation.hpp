#ifndef _COCONUT_PULP_MATH_ROTATION_HPP_
#define _COCONUT_PULP_MATH_ROTATION_HPP_

#include "Angle.hpp"
#include "Vector.hpp"
#include "Quaternion.hpp"
#include "ScalarEqual.hpp"

namespace coconut {
namespace pulp {
namespace math {

class Rotation {
public:

	Rotation(const Angle& angle, const Vec3& axis);

	Rotation(Quat q) noexcept :
		q_(std::move(q))
	{
	}

	Rotation interpolate(const Rotation& other, float factor) const noexcept;

	Vec3 rotate(const Vec3& vector) const noexcept;

private:

	Quat q_;

};

inline Rotation interpolate(const Rotation& lhs, const Rotation& rhs, float factor) {
	return lhs.interpolate(rhs, factor);
}

} // namespace math
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_MATH_ROTATION_HPP_ */
