#include "Primitive.hpp"

#include <cassert>

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::primitive;

namespace /* anonymous */ {

class StoreAsVisitor : public boost::static_visitor<void> {
public:

	StoreAsVisitor(void* buffer, milk::graphics::PixelFormat pixelFormat) :
		buffer_(buffer),
		pixelFormat_(pixelFormat)
	{
	}

	template <class T>
	void operator()(const T& value) const {
		value.storeAs(buffer_, pixelFormat_);
	}

private:

	void* buffer_;

	milk::graphics::PixelFormat pixelFormat_;

};

} // anonymous namespace

Primitive::IncompatiblePixelFormat::IncompatiblePixelFormat(milk::graphics::PixelFormat pixelFormat) :
	coconut_tools::exceptions::RuntimeError("Incompatible pixel format: " + toString(pixelFormat))
{
}

void Primitive::storeAs(void* buffer, milk::graphics::PixelFormat pixelFormat) const {
	data_.apply_visitor(StoreAsVisitor(buffer, pixelFormat));
}
