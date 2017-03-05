#ifndef _COCONUT_PULP_PRIMITIVE_TEXTURECOORDINATE_HPP_
#define _COCONUT_PULP_PRIMITIVE_TEXTURECOORDINATE_HPP_

#include "detail/VectorType.hpp"

namespace coconut {
namespace pulp {
namespace primitive {

class TextureCoordinate : detail::VectorType<2> {
public:

	TextureCoordinate() = default;

	TextureCoordinate(float u, float v) :
		detail::VectorType<2>(u, v, 0.0f, 0.0f)
	{
	}

	float u() const {
		return get<0>();
	}

	float v() const {
		return get<1>();
	}

	using detail::VectorType<2>::storeAs;

};

} // namespace primitive

using primitive::TextureCoordinate;

} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_PRIMITIVE_TEXTURECOORDINATE_HPP_ */
