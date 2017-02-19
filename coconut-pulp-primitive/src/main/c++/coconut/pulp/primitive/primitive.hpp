#ifndef _COCONUT_PULP_PRIMITIVE_PRIMITIVE_HPP_
#define _COCONUT_PULP_PRIMITIVE_PRIMITIVE_HPP_

#include <cstdint>

#include <coconut-tools/enum.hpp>
#include <coconut-tools/serialisation/make-serialisable-macro.hpp>

#include "coconut/milk/graphics/PixelFormat.hpp"

#include "Position.hpp"
#include "Vector.hpp"
#include "Colour.hpp"
#include "TextureCoordinate.hpp"

namespace coconut {
namespace pulp {
namespace primitive {

class Primitive {
public:

	Primitive(float f) :
		type_(Type::SCALAR),
		data_(f)
	{
	}

	Primitive(Colour colour) :
		type_(Type::COLOUR),
		data_(colour)
	{
	}

	void storeAs(void* buffer, milk::graphics::PixelFormat pixelFormat) const; // pixel format? co z macierzami?

private:

	CCN_MEMBER_ENUM(
		Type,
		(SCALAR)
		(POSITION)
		(VECTOR)
		(COLOUR)
		(TEXTURE_COORDINATE)
		);

	Type type_;

	union Data {
		float scalar;
		Position position;
		Vector vector;
		Colour colour;
		TextureCoordinate textureCoordinate;

		Data(float f) :
			scalar(f)
		{
		}

		Data(Colour colour) :
			colour(std::move(colour))
		{
		}

	} data_;

};

} // namespace primitive

using primitive::Primitive;

} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_PRIMITIVE_PRIMITIVE_HPP_ */
