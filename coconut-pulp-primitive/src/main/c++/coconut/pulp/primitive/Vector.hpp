#ifndef _COCONUT_PULP_PRIMITIVE_VECTOR_HPP_
#define _COCONUT_PULP_PRIMITIVE_VECTOR_HPP_

#include "coconut/milk/math/Vector.hpp"

namespace coconut {
namespace pulp {
namespace primitive {

class Vector : milk::math::Vector4d {
public:

	Vector(float x, float y, float z) :
		milk::math::Vector4d(x, y, z, 0.0f)
	{
	}

	using milk::math::Vector4d::x;

	using milk::math::Vector4d::y;

	using milk::math::Vector4d::z;

};

} // namespace primitive

using primitive::Vector;

} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_PRIMITIVE_VECTOR_HPP_ */
