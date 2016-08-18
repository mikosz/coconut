#ifndef _COCONUT_MILK_UTILS_INTEGRALVALUE_HPP_
#define _COCONUT_MILK_UTILS_INTEGRALVALUE_HPP_

#include <type_traits>

namespace coconut {
namespace milk {
namespace utils {

template <class EnumType, std::enable_if_t<std::is_enum<EnumType>::value>* = nullptr>
auto integralValue(EnumType enumValue) {
	return static_cast<std::underlying_type_t<EnumType>>(enumValue);
}

} // namespace utils
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_UTILS_INTEGRALVALUE_HPP_ */
