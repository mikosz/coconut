#ifndef _COCONUT_MILK_FILESYSTEM_FILESYSTEM_HPP_
#define _COCONUT_MILK_FILESYSTEM_FILESYSTEM_HPP_

#include <vector>
#include <string>
#include <iosfwd>
#include <memory>
#include <functional>
#include <unordered_map>

#include <coconut-tools/exceptions/RuntimeError.hpp>

#include "Mount.hpp"
#include "Path.hpp"
#include "types.hpp"

namespace coconut {
namespace milk {
namespace fs {

class Filesystem {
public:

	void mount(AbsolutePath mountPoint, std::unique_ptr<Mount> mountRoot);

	std::vector<std::string> list(const AbsolutePath& path) const;

	IStream open(const AbsolutePath& path) const;

private:

	using WalkOp = std::function<void (const Mount&, const Path&)>;

	using Mounts = std::unordered_map<AbsolutePath, std::unique_ptr<Mount>>;

	Mounts mounts_;

	void walk(const AbsolutePath& path, const WalkOp& walkOp) const;

};

class FailedToReadData : public coconut_tools::exceptions::RuntimeError {
public:

	FailedToReadData(const AbsolutePath& path, const std::runtime_error& cause) :
		coconut_tools::exceptions::RuntimeError("Failed to read data from \"" + path.string() + '\"', cause)
	{
	}

};

RawData readRawData(const AbsolutePath& path, std::istream& is);

} // namespace fs
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_FILESYSTEM_FILESYSTEM_HPP_ */
