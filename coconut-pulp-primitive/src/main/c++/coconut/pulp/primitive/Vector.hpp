#ifndef _COCONUT_PULP_PRIMITIVE_VECTOR_HPP_
#define _COCONUT_PULP_PRIMITIVE_VECTOR_HPP_

#include <iosfwd>

#include <coconut-tools/serialisation.hpp>

#include <boost/operators.hpp>
#include "detail/VectorType.hpp"

namespace coconut {
namespace pulp {
namespace primitive {

class Vector :
	boost::additive<Vector>,
	detail::VectorType<4>
{
public:

	Vector() = default;

	Vector(const milk::math::Vector3d& vector) :
		detail::VectorType<4>(vector.x(), vector.y(), vector.z(), 0.0f)
	{
	}

	Vector(const milk::math::Vector4d& vector) :
		detail::VectorType<4>(vector.x(), vector.y(), vector.z(), 0.0f)
	{
		assert(vector.w() < 0.0f + EPSILON); // TODO: replace with some "between" function
		assert(vector.w() > 0.0f - EPSILON);
	}

	Vector(float x, float y, float z) :
		detail::VectorType<4>(x, y, z, 0.0f)
	{
	}

	Vector& operator+=(const Vector& rhs) {
		static_cast<detail::VectorType<4>&>(*this) += rhs;
		return *this;
	}

	Vector& operator-=(const Vector& rhs) {
		static_cast<detail::VectorType<4>&>(*this) -= rhs;
		return *this;
	}

	Vector cross(const Vector& rhs) const {
		auto lhsV3 = milk::math::Vector3d(x(), y(), z());
		const auto rhsV3 = milk::math::Vector3d(rhs.x(), rhs.y(), rhs.z());
		lhsV3.crossEq(rhsV3);
		return Vector(rhsV3.x(), rhsV3.y(), rhsV3.z());
	}

	using detail::VectorType<4>::normalise;

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

	friend std::ostream& operator<<(std::ostream& os, const Vector& vector) {
		return os << static_cast<const detail::VectorType<4>&>(vector);
	}

};

CCN_MAKE_SERIALISABLE(SerialiserType, serialiser, Vector, vector) {
	serialiser(SerialiserType::Label("x"), vector.x());
	serialiser(SerialiserType::Label("y"), vector.y());
	serialiser(SerialiserType::Label("z"), vector.z());
}

} // namespace primitive

using primitive::Vector;

} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_PRIMITIVE_VECTOR_HPP_ */
