#ifndef _COCONUT_PULP_PRIMITIVE_POSITION_HPP_
#define _COCONUT_PULP_PRIMITIVE_POSITION_HPP_

#include "coconut/milk/math/Vector.hpp"

namespace coconut {
namespace pulp {
namespace primitive {

class Position : milk::math::Vector4d {
public:

	Position(float x, float y, float z) :
		milk::math::Vector4d(x, y, z, 1.0f)
	{
	}

	using milk::math::Vector4d::x;

	using milk::math::Vector4d::y;

	using milk::math::Vector4d::z;

};

} // namespace primitive

using primitive::Position;

} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_PRIMITIVE_POSITION_HPP_ */
