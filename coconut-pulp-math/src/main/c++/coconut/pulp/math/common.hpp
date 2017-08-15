#ifndef _COCONUT_PULP_MATH_COMMON_HPP_
#define _COCONUT_PULP_MATH_COMMON_HPP_

namespace coconut {
namespace pulp {
namespace math {

template <class T>
inline T lerp(T from, T to, float factor) { 
	return (1.0f - factor) * from + factor * to; 
}

} // namespace math

using math::lerp;

} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_MATH_COMMON_HPP_ */
