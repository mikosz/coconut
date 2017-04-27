#ifndef _COCONUT_PULP_MATH_HOMOGENEOUSCOORDS_HPP_
#define _COCONUT_PULP_MATH_HOMOGENEOUSCOORDS_HPP_

#include "Vector.hpp"
#include "ScalarEqual.hpp"

namespace coconut {
namespace pulp {
namespace math {

class HomogeneousCoords {
private:

	struct Point {
	};

	struct Vector {
	};

public:

	static constexpr Point POINT{};

	static constexpr Vector VECTOR{};

	constexpr HomogeneousCoords(Vec3 point, Point) noexcept : // TODO: there's a bit too much typing to get this
		vector_(point, 1.0f)
	{
	}

	constexpr HomogeneousCoords(Vec3 vector, Vector) noexcept :
		vector_(vector, 0.0f)
	{
	}

	constexpr HomogeneousCoords(Vec4 vector) noexcept :
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

} // namespace math

using math::HomogeneousCoords;

} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_MATH_HOMOGENEOUSCOORDS_HPP_ */
