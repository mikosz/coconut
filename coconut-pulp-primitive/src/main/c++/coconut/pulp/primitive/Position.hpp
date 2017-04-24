#ifndef _COCONUT_PULP_PRIMITIVE_POSITION_HPP_
#define _COCONUT_PULP_PRIMITIVE_POSITION_HPP_

#include <boost/operators.hpp>

#include <coconut-tools/serialisation.hpp>

#include "detail/VectorType.hpp"
#include "Vector.hpp"

namespace coconut {
namespace pulp {
namespace primitive {

class Position :
	boost::additive<Position, Vector>,
	detail::VectorType<4>
{
public:

	Position() = default;

	Position(const pulp::math::Vec3& vector) :
		detail::VectorType<4>(vector.x(), vector.y(), vector.z(), 1.0f)
	{
	}

	Position(const pulp::math::Vec4& vector) :
		detail::VectorType<4>(vector.x(), vector.y(), vector.z(), 1.0f)
	{
	}

	Position(float x, float y, float z) :
		detail::VectorType<4>(x, y, z, 1.0f)
	{
	}

	Position& operator+=(const primitive::Vector& rhs) {
		static_cast<detail::VectorType<4>&>(*this) += rhs;
		return *this;
	}

	Position& operator-=(const primitive::Vector& rhs) {
		static_cast<detail::VectorType<4>&>(*this) -= rhs;
		return *this;
	}

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

	friend primitive::Vector operator-(const Position& lhs, const Position& rhs) {
		return primitive::Vector(lhs.x() - rhs.x(), lhs.y() - rhs.y(), lhs.z() - rhs.z());
	}

	friend std::ostream& operator<<(std::ostream& os, const Position& position) {
		return os << static_cast<const detail::VectorType<4>&>(position);
	}

	friend class primitive::Vector;

};

CCN_MAKE_SERIALISABLE(SerialiserType, serialiser, Position, position) {
	serialiser(SerialiserType::Label("x"), position.x());
	serialiser(SerialiserType::Label("y"), position.y());
	serialiser(SerialiserType::Label("z"), position.z());
}

} // namespace primitive

using primitive::Position;

} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_PRIMITIVE_POSITION_HPP_ */
