#ifndef _COCONUT_PULP_MATH_PLANE_HPP_
#define _COCONUT_PULP_MATH_PLANE_HPP_

#include "Vector.hpp"
#include "ScalarEqual.hpp"

namespace coconut {
namespace pulp {
namespace math {

class Plane {
public:

	Plane(Vec3 normal, const Vec3& point) noexcept {
		normal.normalise();
		equation_ = Vec4(normal, -normal.dot(point));
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

	const float signedDistanceToPoint(const Vec3& point) const noexcept {
		return equation_.dot(Vec4(point, 1.0f));
	}

private:

	Vec4 equation_; // <Nx, Ny, Nz, D>

};

} // namespace math

using math::Plane;

} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_MATH_PLANE_HPP_ */
