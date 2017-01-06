#ifndef _COCONUT_MILK_FILESYSTEM_PATH_HPP_
#define _COCONUT_MILK_FILESYSTEM_PATH_HPP_

#include <string>
#include <iosfwd>

#include <boost/operators.hpp>
#include <boost/optional.hpp> // TODO: change to std::optional after switching to c++ latest
#include <boost/filesystem/path.hpp>

#include <coconut-tools/exceptions/RuntimeError.hpp>

namespace coconut {
namespace milk {
namespace fs {

class Path :
	public boost::dividable<Path>,
	public boost::equality_comparable<Path>
{
public:

	Path() = default;

	Path(boost::filesystem::path physicalPath);

	Path(const char* pathString);

	Path(const std::string& pathString);

	const bool operator==(const Path& other) const noexcept {
		return unifiedPath_ == other.unifiedPath_;
	}

	Path& operator/=(const Path& tail);

	bool empty() const noexcept {
		return unifiedPath_.empty();
	}

	size_t hashValue() const noexcept {
		return std::hash<std::string>()(unifiedPath_.string());
	}

	bool absolute() const noexcept {
		return unifiedPath_.has_root_directory();
	}

	bool relative() const noexcept {
		return !absolute();
	}

	Path parent() const {
		return unifiedPath_.parent_path();
	}

	Path base() const {
		return unifiedPath_.filename();
	}

	const boost::filesystem::path& physicalPath() const noexcept {
		return unifiedPath_;
	}

	const std::string string() const {
		return unifiedPath_.string();
	}

private:

	boost::filesystem::path unifiedPath_;

};

class InvalidPath : public coconut_tools::exceptions::RuntimeError {
public:

	using coconut_tools::exceptions::RuntimeError::RuntimeError;

};

inline std::ostream& operator<<(std::ostream& os, const Path& path) {
	return os << path.string();
}

class AbsolutePath : public Path {
public:

	AbsolutePath() = default;

	AbsolutePath(boost::filesystem::path physicalPath);

	AbsolutePath(const Path& path);

	AbsolutePath(const char* pathString);

	AbsolutePath(const std::string& pathString);

	boost::optional<Path> relativeTo(const AbsolutePath& parent) const;

};

} // namespace fs
} // namespace milk
} // namespace coconut

namespace std {

template <>
struct hash<coconut::milk::fs::Path> {

	size_t operator()(const coconut::milk::fs::Path& path) const noexcept {
		return path.hashValue();
	}

};

template <>
struct hash<coconut::milk::fs::AbsolutePath> : public hash<coconut::milk::fs::Path> {
};

} // namespace std

#endif /* _COCONUT_MILK_FILESYSTEM_PATH_HPP_ */
