#ifndef _COCONUT_PULP_PRIMITIVE_COLOUR_HPP_
#define _COCONUT_PULP_PRIMITIVE_COLOUR_HPP_

#include "coconut/milk/math/Vector.hpp"

namespace coconut {
namespace pulp {
namespace primitive {

class Colour : milk::math::Vector4d {
public:

	Colour(float r, float g, float b, float a = 1.0f) :
		milk::math::Vector4d(r, g, b, a)
	{
	}

	float r() const {
		return x();
	}

	float& r() {
		return x();
	}

	float g() const {
		return y();
	}

	float& g() {
		return y();
	}

	float b() const {
		return z();
	}

	float& b() {
		return z();
	}

	float a() const {
		return w();
	}

	float& a() {
		return w();
	}

};

} // namespace primitive

using primitive::Colour;

} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_PRIMITIVE_COLOUR_HPP_ */
