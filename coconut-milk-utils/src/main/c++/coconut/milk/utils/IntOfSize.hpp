#ifndef _COCONUT_MILK_UTILS_HPP_
#define _COCONUT_MILK_UTILS_HPP_

#include <cstdint>

namespace coconut {
namespace milk {
namespace utils {

// TODO: probably useless in c++11 with accessible underlying types
template <int>
struct IntOfSize {

};

template <>
struct IntOfSize<1> {
	typedef std::int8_t Signed;
	typedef std::uint8_t Unsigned;
};

template <>
struct IntOfSize<2> {
	typedef std::int16_t Signed;
	typedef std::uint16_t Unsigned;
};

template <>
struct IntOfSize<4> {
	typedef std::int32_t Signed;
	typedef std::uint32_t Unsigned;
};

} // namespace utils
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_UTILS_HPP_ */
