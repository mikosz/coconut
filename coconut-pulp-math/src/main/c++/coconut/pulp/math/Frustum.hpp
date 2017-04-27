#ifndef _COCONUT_PULP_MATH_FRUSTUM_HPP_
#define _COCONUT_PULP_MATH_FRUSTUM_HPP_

#include <array>

#include "Plane.hpp"

namespace coconut {
namespace pulp {
namespace math {

class Frustum {
public:



private:

	enum class PlaneIndex {
		NEAR,
		FAR,
		LEFT,
		RIGHT,
		TOP,
		BOTTOM,
	};

	std::array<Plane, 6> planes_;

};

} // namespace math

using math::Frustum;

} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_MATH_FRUSTUM_HPP_ */
