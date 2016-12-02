#ifndef _COCONUT_MILK_FILESYSTEM_OPERATIONS_HPP_
#define _COCONUT_MILK_FILESYSTEM_OPERATIONS_HPP_

#include <iosfwd>

#include <coconut-tools/exceptions/RuntimeError.hpp>

#include "types.hpp"

namespace coconut {
namespace milk {
namespace filesystem {

class FailedToReadData : public coconut_tools::exceptions::RuntimeError {
public:

	FailedToReadData(const Path& path, const std::runtime_error& cause) :
		coconut_tools::exceptions::RuntimeError("Failed to read data from \"" + path.string() + "\"", cause)
	{
	}

};

RawData readRawData(const Path& path, std::istream& is);

} // namespace filesystem
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_FILESYSTEM_OPERATIONS_HPP_ */
