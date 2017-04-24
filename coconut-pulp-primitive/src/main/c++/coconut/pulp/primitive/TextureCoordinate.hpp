#ifndef _COCONUT_PULP_PRIMITIVE_TEXTURECOORDINATE_HPP_
#define _COCONUT_PULP_PRIMITIVE_TEXTURECOORDINATE_HPP_

#include <coconut-tools/serialisation.hpp>

#include "detail/VectorType.hpp"

namespace coconut {
namespace pulp {
namespace primitive {

class TextureCoordinate : detail::VectorType<2> {
public:

	TextureCoordinate() = default;

	TextureCoordinate(float u, float v) :
		detail::VectorType<2>(u, v)
	{
	}

	float& u() {
		return get<0>();
	}

	float u() const {
		return get<0>();
	}

	float& v() {
		return get<1>();
	}

	float v() const {
		return get<1>();
	}

	using detail::VectorType<2>::storeAs;

};

CCN_MAKE_SERIALISABLE(SerialiserType, serialiser, TextureCoordinate, textureCoordinate) {
	serialiser(SerialiserType::Label("u"), textureCoordinate.u());
	serialiser(SerialiserType::Label("v"), textureCoordinate.v());
}

} // namespace primitive

using primitive::TextureCoordinate;

} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_PRIMITIVE_TEXTURECOORDINATE_HPP_ */
