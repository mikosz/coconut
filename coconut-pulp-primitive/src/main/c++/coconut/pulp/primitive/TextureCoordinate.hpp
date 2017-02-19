#ifndef _COCONUT_PULP_PRIMITIVE_TEXTURECOORDINATE_HPP_
#define _COCONUT_PULP_PRIMITIVE_TEXTURECOORDINATE_HPP_

#include "coconut/milk/math/Vector.hpp"

namespace coconut {
namespace pulp {
namespace primitive {

class TextureCoordinate : milk::math::Vector2d {
public:

	TextureCoordinate(float u, float v) :
		milk::math::Vector2d(u, v)
	{
	}

	float u() const {
		return x();
	}

	float v() const {
		return y();
	}

};

} // namespace primitive

using primitive::TextureCoordinate;

} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_PRIMITIVE_TEXTURECOORDINATE_HPP_ */
