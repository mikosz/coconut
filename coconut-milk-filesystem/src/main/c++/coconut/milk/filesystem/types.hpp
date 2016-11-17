#ifndef _COCONUT_MILK_FILESYSTEM_TYPES_HPP_
#define _COCONUT_MILK_FILESYSTEM_TYPES_HPP_

#include <cstdint>
#include <vector>
#include <iosfwd>
#include <memory>

#include <boost/filesystem/path.hpp>

namespace coconut {
namespace milk {
namespace filesystem {

using Path = boost::filesystem::path;
using Byte = std::uint8_t;
using RawData = std::vector<Byte>;
using IStream = std::unique_ptr<std::istream>;

} // namespace filesystem
} // namespace milk
} // namespace coconut

namespace std {

template <>
struct hash<coconut::milk::filesystem::Path> {

	size_t operator()(const coconut::milk::filesystem::Path& path) const {
		return std::hash<std::string>()(path.string());
	}

};

} // namespace std

#endif /* _COCONUT_MILK_FILESYSTEM_TYPES_HPP_ */
