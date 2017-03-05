#ifndef _COCONUT_PULP_PRIMITIVE_VECTOR_HPP_
#define _COCONUT_PULP_PRIMITIVE_VECTOR_HPP_

#include "detail/VectorType.hpp"

namespace coconut {
namespace pulp {
namespace primitive {

class Vector : detail::VectorType<4> {
public:

	Vector() = default;

	Vector(float x, float y, float z) :
		detail::VectorType<4>(x, y, z, 0.0f)
	{
	}

	float x() const {
		return get<0>();
	}

	float y() const {
		return get<1>();
	}

	float z() const {
		return get<2>();
	}

	using detail::VectorType<4>::storeAs;

private:

	friend class Position;

};

} // namespace primitive

using primitive::Vector;

} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_PRIMITIVE_VECTOR_HPP_ */
