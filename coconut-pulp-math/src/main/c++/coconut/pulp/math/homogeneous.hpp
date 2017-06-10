#ifndef _COCONUT_PULP_MATH_HOMOGENEOUS_HPP_
#define _COCONUT_PULP_MATH_HOMOGENEOUS_HPP_

#include "Vector.hpp"
#include "ScalarEqual.hpp"

namespace coconut {
namespace pulp {
namespace math {

class HomogeneousCoordinates {
public:

	constexpr HomogeneousCoordinates(Vec4 vector) noexcept :
		vector_(std::move(vector))
	{
	}

	constexpr operator Vec4() const noexcept {
		return vector_;
	}

	constexpr operator const Vec4&() const noexcept {
		return vector_;
	}

	Vec3 to3dSpace() const noexcept {
		if (ScalarEqual<float>()(vector_.w(), 0.0f)) {
			return vector_.xyz();
		} else {
			return vector_.xyz() / vector_.w();
		}
	}

	constexpr const Vec4& vector() const noexcept {
		return vector_;
	}

	Vec4& vector() noexcept {
		return vector_;
	}

private:

	Vec4 vector_;

};

class HomogeneousPoint : public HomogeneousCoordinates {
public:

	explicit constexpr HomogeneousPoint(Vec3 point) noexcept :
		HomogeneousCoordinates(Vec4(point, 1.0f))
	{
	}

};

class HomogeneousVector : public HomogeneousCoordinates {
public:

	explicit constexpr HomogeneousVector(Vec3 vector) noexcept :
		HomogeneousCoordinates(Vec4(vector, 0.0f))
	{
	}

};

} // namespace math
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_MATH_HOMOGENEOUS_HPP_ */
