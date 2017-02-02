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

template <class T>
size_t numberOfBitsOn(T value) {
	size_t bitsOn = 0;

	while (value != 0) {
		auto minusOne = value - 1;
		value = value & minusOne;
		++bitsOn;
	}

	return bitsOn;
}

} // namespace utils
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_UTILS_BITS_HPP_ */
