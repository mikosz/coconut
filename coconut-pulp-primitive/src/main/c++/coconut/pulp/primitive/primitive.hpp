#ifndef _COCONUT_PULP_PRIMITIVE_PRIMITIVEID_HPP_
#define _COCONUT_PULP_PRIMITIVE_PRIMITIVEID_HPP_

#include <cstdint>
#include <type_traits>

#include <coconut-tools/enum.hpp>

#include "coconut/milk/graphics/PixelFormat.hpp"

#include "Position.hpp"

namespace coconut {
namespace pulp {
namespace primitive {

CCN_ENUM(
	PrimitiveType,
	(POSITION)
	);

class Primitive {
public:

	PrimitiveType type() const {
		return type_;
	}

	void storeAs(void* buffer, milk::graphics::PixelFormat pixelFormat) const; // pixel format? co z macierzami?

private:

	PrimitiveType type_;

	union {
		Position position;
	} primitiveHolder_;

};

// static_assert(std::is_trivial<Position>::value, "Position type must be trivial");

} // namespace primitive
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_PRIMITIVE_PRIMITIVEID_HPP_ */
