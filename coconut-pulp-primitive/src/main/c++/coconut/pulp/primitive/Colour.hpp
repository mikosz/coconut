#ifndef _COCONUT_PULP_PRIMITIVE_COLOUR_HPP_
#define _COCONUT_PULP_PRIMITIVE_COLOUR_HPP_

#include <coconut-tools/serialisation.hpp>

#include "detail/VectorType.hpp"

namespace coconut {
namespace pulp {
namespace primitive {

class Colour : detail::VectorType<4> {
public:

	Colour() = default;

	Colour(float r, float g, float b, float a = 1.0f) :
		detail::VectorType<4>(r, g, b, a)
	{
	}

	float& r() {
		return get<0>();
	}

	float r() const {
		return get<0>();
	}

	float& g() {
		return get<1>();
	}

	float g() const {
		return get<1>();
	}

	float& b() {
		return get<2>();
	}

	float b() const {
		return get<2>();
	}

	float& a() {
		return get<3>();
	}

	float a() const {
		return get<3>();
	}

	using detail::VectorType<4>::storeAs;

};

CCN_MAKE_SERIALISABLE(SerialiserType, serialiser, Colour, colour) {
	serialiser(SerialiserType::Label("r"), vec.r());
	serialiser(SerialiserType::Label("g"), vec.g());
	serialiser(SerialiserType::Label("b"), vec.b());
	serialiser(SerialiserType::Label("a"), vec.a());
}

} // namespace primitive

using primitive::Colour;

} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_PRIMITIVE_COLOUR_HPP_ */
