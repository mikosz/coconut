#ifndef _COCONUT_PULP_PRIMITIVE_VECTOR_HPP_
#define _COCONUT_PULP_PRIMITIVE_VECTOR_HPP_

#include <iosfwd>

#include <coconut-tools/serialisation.hpp>

#include <boost/operators.hpp>
#include "detail/VectorType.hpp"

namespace coconut {
namespace pulp {
namespace primitive {

class Vector : // TODO: clashes with pulp::math::Vector name
	boost::additive<Vector>,
	detail::VectorType<4>
{
public:

	Vector() = default;

	Vector(const Vec3& vector) :
		detail::VectorType<4>(vector.x(), vector.y(), vector.z(), 0.0f)
	{
	}

	Vector(const Vec4& vector) :
		detail::VectorType<4>(vector.x(), vector.y(), vector.z(), 0.0f)
	{
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
		auto lhsV3 = pulp::math::Vec3(x(), y(), z());
		const auto rhsV3 = pulp::math::Vec3(rhs.x(), rhs.y(), rhs.z());
		lhsV3.crossEq(rhsV3);
		return Vector(rhsV3.x(), rhsV3.y(), rhsV3.z());
	}

	using detail::VectorType<4>::normalise;

	float& x() {
		return get<0>();
	}

	float x() const {
		return get<0>();
	}

	float& y() {
		return get<1>();
	}

	float y() const {
		return get<1>();
	}

	float& z() {
		return get<2>();
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
