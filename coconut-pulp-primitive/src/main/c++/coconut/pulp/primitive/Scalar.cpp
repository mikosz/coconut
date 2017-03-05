#include "Scalar.hpp"

#include "Primitive.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::primitive;

void Scalar::storeAs(void* buffer, milk::graphics::PixelFormat pixelFormat) const {
	throw Primitive::IncompatiblePixelFormat(pixelFormat);
}
