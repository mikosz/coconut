#ifndef _COCONUT_PULP_MATH_COMMON_HPP_
#define _COCONUT_PULP_MATH_COMMON_HPP_

namespace coconut {
namespace pulp {
namespace math {

template <class T>
T lerp(T level, T from, T to) { 
	return level * to + (T(1.0) - level) * from; 
}

} // namespace math

using math::lerp;

} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_MATH_COMMON_HPP_ */
