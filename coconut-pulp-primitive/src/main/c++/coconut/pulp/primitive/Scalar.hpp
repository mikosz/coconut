#ifndef _COCONUT_PULP_PRIMITIVE_SCALAR_HPP_
#define _COCONUT_PULP_PRIMITIVE_SCALAR_HPP_

#include <coconut-tools/serialisation.hpp>

#include "coconut/milk/graphics/PixelFormat.hpp"

namespace coconut {
namespace pulp {
namespace primitive {

class Scalar {
public:

	Scalar() = default;

	/* implicit */ Scalar(float /*value*/)
	{
	}

	float value() const {
		return value_;
	}

	float& value() {
		return value_;
	}

	void storeAs(void* buffer, milk::graphics::PixelFormat pixelFormat) const;

private:

	float value_;

};

CT_MAKE_SERIALISABLE(SerialiserType, serialiser, Scalar, scalar) {
	serialiser(SerialiserType::Label("value"), scalar.value());
}

} // namespace primitive

using primitive::Scalar;

} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_PRIMITIVE_SCALAR_HPP_ */
