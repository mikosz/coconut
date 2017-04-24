#ifndef _COCONUT_PULP_PRIMITIVE_PRIMITIVE_HPP_
#define _COCONUT_PULP_PRIMITIVE_PRIMITIVE_HPP_

#include <boost/variant.hpp>

#include <coconut-tools/exceptions/RuntimeError.hpp>
#include <coconut-tools/serialisation.hpp>

#include "coconut/milk/graphics/PixelFormat.hpp"

#include "Position.hpp"
#include "Vector.hpp"
#include "Colour.hpp"
#include "Scalar.hpp"
#include "TextureCoordinate.hpp"

namespace coconut {
namespace pulp {
namespace primitive {

class Primitive {
public:

	class IncompatiblePixelFormat : public coconut_tools::exceptions::RuntimeError {
	public:
	
		IncompatiblePixelFormat(milk::graphics::PixelFormat pixelFormat);

	};

	Primitive() = default;

	/* implicit */ Primitive(Scalar scalar) :
		data_(scalar)
	{
	}

	/* implicit */ Primitive(Position position) :
		data_(position)
	{
	}

	/* implicit */ Primitive(Vector vector) :
		data_(vector)
	{
	}

	/* implicit */ Primitive(Colour colour) :
		data_(colour)
	{
	}

	/* implicit */ Primitive(TextureCoordinate textureCoordinate) :
		data_(textureCoordinate)
	{
	}

	void storeAs(void* buffer, milk::graphics::PixelFormat pixelFormat) const; // pixel format? co z macierzami?

private:

	using Data = boost::variant< // TODO: replace with std::variant when available
		Scalar,
		Position,
		Vector,
		Colour,
		TextureCoordinate
		>;
	
	Data data_;

	friend void serialise(coconut_tools::Serialiser& serialiser, const Primitive& aPrimitive);

	friend void serialise(coconut_tools::Deserialiser& serialiser, Primitive& aPrimitive);

};

} // namespace primitive

using primitive::Primitive;

} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_PRIMITIVE_PRIMITIVE_HPP_ */
