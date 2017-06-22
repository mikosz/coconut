#ifndef _COCONUT_PULP_MATH_SCALAREQUAL_HPP_
#define _COCONUT_PULP_MATH_SCALAREQUAL_HPP_

namespace coconut {
namespace pulp {
namespace math {

template <class ScalarType>
struct ScalarEqual {

	constexpr bool operator()(ScalarType lhs, ScalarType rhs) const noexcept {
		return std::equal_to<ScalarType>()(lhs, rhs);
	}

};

template <>
struct ScalarEqual<float> {

	constexpr bool operator()(float lhs, float rhs) const noexcept {
		return std::abs(lhs - rhs) < 1.0f / 10'000.0f;
	}

};

} // namespace math
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_MATH_SCALAREQUAL_HPP_ */
