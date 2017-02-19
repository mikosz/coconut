#include "Primitive.hpp"

#include <cassert>

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::primitive;

void Primitive::storeAs(void* buffer, milk::graphics::PixelFormat pixelFormat) const {
	switch (type_) {
	case Type::POSITION:
		primitiveHolder_.position.storeAs(buffer, pixelFormat);
		break;
	case Type::VECTOR:
		primitiveHolder_.vector.storeAs(buffer, pixelFormat);
		break;
	case Type::COLOUR:
		primitiveHolder_.colour.storeAs(buffer, pixelFormat);
		break;
	default:
		assert(!"Unexpected primitive type");
	}
}
