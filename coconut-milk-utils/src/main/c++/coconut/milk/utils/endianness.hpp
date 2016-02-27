#ifndef _COCONUT_MILK_UTILS_ENDIANNESS_HPP_
#define _COCONUT_MILK_UTILS_ENDIANNESS_HPP_

#include <cstdint>

namespace coconut {
namespace milk {
namespace utils {

namespace detail {

union {
	std::uint16_t value;
	unsigned char bytes[2];
} constexpr static one{ 1 };

} // namespace detail

constexpr bool isBigEndian() {
	return detail::one.bytes[1] == 1;
}

template <class T>
T changeEndianness(T value) { // TODO: should only accept POD values
	T result;
	auto* writer = reinterpret_cast<std::uint8_t*>(&result);
	const auto* firstByte = reinterpret_cast<const std::uint8_t*>(&value);
	for (const auto* byte = firstByte + sizeof(value); byte > firstByte; --byte) {
		*writer++ = *(byte - 1);
	}

	return result;
}

} // namespace utils
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_UTILS_ENDIANNESS_HPP_ */