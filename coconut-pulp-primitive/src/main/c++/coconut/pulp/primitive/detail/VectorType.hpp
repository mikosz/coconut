#ifndef _COCONUT_PULP_PRIMITIVE_DETAIL_VECTORTYPE_HPP_
#define _COCONUT_PULP_PRIMITIVE_DETAIL_VECTORTYPE_HPP_

#include "coconut/milk/graphics/PixelFormat.hpp"

#include "coconut/milk/math/Vector.hpp"

namespace coconut {
namespace pulp {
namespace primitive {
namespace detail {

template <size_t DIMENSIONS>
class VectorSuperclass;

template <>
class VectorSuperclass<2> {
	using Type = milk::math::Vector2d;
};

template <>
class VectorSuperclass<3> {
	using Type = milk::math::Vector3d;
};

template <>
class VectorSuperclass<4> {
	using Type = milk::math::Vector4d;
};

template <size_t DIMENSIONS>
class VectorType : public VectorSuperclass<DIMENSIONS>::Type {
public:

	using typename VectorSuperclass<DIMENSIONS>::Type::Type;

	void storeAs(void* buffer, milk::graphics::PixelFormat pixelFormat) const;

};

} // namespace detail
} // namespace primitive
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_PRIMITIVE_DETAIL_VECTORTYPE_HPP_ */
