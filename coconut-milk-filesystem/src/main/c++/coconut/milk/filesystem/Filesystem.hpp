#ifndef _COCONUT_MILK_FILESYSTEM_FILESYSTEM_HPP_
#define _COCONUT_MILK_FILESYSTEM_FILESYSTEM_HPP_

#include <vector>
#include <iosfwd>

#include <coconut-tools/exceptions/RuntimeError.hpp>

#include "types.hpp"

namespace coconut {
namespace milk {
namespace filesystem {

class Filesystem {
public:



	void mount(Path directory);

private:

	std::vector<Path> mounts_;

};

class FailedToReadData : public coconut_tools::exceptions::RuntimeError {
public:

	FailedToReadData(const Path& path, const std::runtime_error& cause) :
		coconut_tools::exceptions::RuntimeError("Failed to read data from \"" + path.string() + "\"", cause)
	{
	}

};

std::shared_ptr<const RawData> readRawData(const Path& path, const IStream& is);

} // namespace filesystem
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_FILESYSTEM_FILESYSTEM_HPP_ */
