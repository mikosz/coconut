#ifndef _COCONUT_MILK_FILESYSTEM_TYPES_HPP_
#define _COCONUT_MILK_FILESYSTEM_TYPES_HPP_

#include <cstdint>
#include <vector>
#include <iosfwd>
#include <memory>

namespace coconut {
namespace milk {
namespace fs {

using Byte = std::uint8_t;
using RawData = std::vector<Byte>;
using IStream = std::unique_ptr<std::istream>;
using OStream = std::unique_ptr<std::ostream>;

} // namespace fs
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_FILESYSTEM_TYPES_HPP_ */
