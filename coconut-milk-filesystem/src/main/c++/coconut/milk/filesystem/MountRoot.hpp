#ifndef _COCONUT_MILK_FILESYSTEM_MOUNTROOT_HPP_
#define _COCONUT_MILK_FILESYSTEM_MOUNTROOT_HPP_

#include <vector>
#include <string>

#include "types.hpp"

namespace coconut {
namespace milk {
namespace filesystem {

class MountRoot {
public:

	virtual ~MountRoot() = default;

	virtual std::vector<std::string> list(const Path& path) const = 0;

	virtual IStream open(const Path& path) const = 0;

};

} // namespace filesystem
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_FILESYSTEM_MOUNTROOT_HPP_ */
