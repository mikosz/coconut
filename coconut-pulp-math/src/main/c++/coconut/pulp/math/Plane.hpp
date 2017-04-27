#ifndef _COCONUT_PULP_MATH_PLANE_HPP_
#define _COCONUT_PULP_MATH_PLANE_HPP_

#include "Vector.hpp"
#include "ScalarEqual.hpp"

namespace coconut {
namespace pulp {
namespace math {

class Plane {
public:

	enum class Side {
		FRONT,
		BACK,
		ON
	};

	Plane(const Vec3& normal, const Vec3& point) noexcept :
		equation_(normal.normalised(), -normal.dot(point))
	{
	}

	Plane(const Vec3& normal, float distanceFromOrigin) noexcept :
		equation_(normal.normalised(), distanceFromOrigin)
	{
	}

	const Vec3 normal() const noexcept {
		return equation_.xyz();
	}

	const float signedDistanceToOrigin() const noexcept {
		return equation_.w();
	}

	Side testPosition(const Vec3& point) const noexcept {
		const auto dot = equation_.dot(Vec4(point, 1.0f));
		if (ScalarEqual<float>()(dot, 0.0f)) {
			return Side::ON;
		} else if (dot > 0.0f) {
			return Side::FRONT;
		} else {
			return Side::BACK;
		}
	}

private:

	Vec4 equation_; // <Nx, Ny, Nz, D>

};

} // namespace math

using math::Plane;

} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_MATH_PLANE_HPP_ */
