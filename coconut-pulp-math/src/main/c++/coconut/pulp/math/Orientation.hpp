#ifndef _COCONUT_PULP_MATH_ORIENTATION_HPP_
#define _COCONUT_PULP_MATH_ORIENTATION_HPP_

#include "Vector.hpp"
#include "Rotation.hpp"

namespace coconut {
namespace pulp {
namespace math {

class Orientation {
public:

private:

	Vec3 position_;

	Rotation rotation_;

};

} // namespace math

using math::Orientation;

} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_MATH_ORIENTATION_HPP_ */
