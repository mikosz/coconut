#ifndef _COCONUT_PULP_PRIMITIVE_DETAIL_VECTORTYPE_HPP_
#define _COCONUT_PULP_PRIMITIVE_DETAIL_VECTORTYPE_HPP_

#include "coconut/milk/graphics/PixelFormat.hpp"

#include "coconut/pulp/math/Vector.hpp"

namespace coconut {
namespace pulp {
namespace primitive {
namespace detail {

template <size_t DIMENSIONS>
class VectorType : public milk::math::Vector<DIMENSIONS> {
public:

	using milk::math::Vector<DIMENSIONS>::Vector;

	void storeAs(void* buffer, milk::graphics::PixelFormat pixelFormat) const;

};

} // namespace detail
} // namespace primitive
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_PRIMITIVE_DETAIL_VECTORTYPE_HPP_ */
