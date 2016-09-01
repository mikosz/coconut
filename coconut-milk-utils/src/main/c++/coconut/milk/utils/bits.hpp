#ifndef _COCONUT_MILK_UTILS_BITS_HPP_
#define _COCONUT_MILK_UTILS_BITS_HPP_

#include <cassert>

namespace coconut {
namespace milk {
namespace utils {

template <class T, class U>
T roundUpToMultipleOf(T value, U multiple) {
	assert(multiple != 0);

	T remainder = value % multiple;

	if (remainder == 0) {
		return value;
	} else {
		return value + multiple - remainder;
	}
}

} // namespace utils
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_UTILS_BITS_HPP_ */
