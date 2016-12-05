#ifndef _COCONUT_MILK_FILESYSTEM_TYPES_HPP_
#define _COCONUT_MILK_FILESYSTEM_TYPES_HPP_

#include <cstdint>
#include <vector>
#include <iosfwd>
#include <memory>

#include <boost/filesystem/path.hpp>

#include <coconut-tools/exceptions/RuntimeError.hpp>

namespace coconut {
namespace milk {
namespace fs {

using Path = boost::filesystem::path;
using Byte = std::uint8_t;
using RawData = std::vector<Byte>;
using IStream = std::unique_ptr<std::istream>;

class InvalidPath : public coconut_tools::exceptions::RuntimeError {
public:

	using coconut_tools::exceptions::RuntimeError::RuntimeError;

};

} // namespace fs
} // namespace milk
} // namespace coconut

namespace std {

template <>
struct hash<coconut::milk::fs::Path> : std::hash<std::string> {

	size_t operator()(const coconut::milk::fs::Path& path) const {
		return reinterpret_cast<const std::hash<std::string>&>(*this)(path.string());
	}

};

} // namespace std

#endif /* _COCONUT_MILK_FILESYSTEM_TYPES_HPP_ */
