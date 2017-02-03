#ifndef _COCONUT_PULP_PRIMITIVE_POSITION_HPP_
#define _COCONUT_PULP_PRIMITIVE_POSITION_HPP_

#include "coconut/milk/math/Vector.hpp"

namespace coconut {
namespace pulp {
namespace primitive {

class Position : milk::math::Vector4d {
public:

	using milk::math::Vector4d::Vector4d;

	milk::math::Vector4d::x;

	milk::math::Vector4d::y;

	milk::math::Vector4d::z;

};

} // namespace primitive
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_PRIMITIVE_POSITION_HPP_ */
