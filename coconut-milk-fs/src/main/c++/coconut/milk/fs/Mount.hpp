#ifndef _COCONUT_MILK_FILESYSTEM_MOUNT_HPP_
#define _COCONUT_MILK_FILESYSTEM_MOUNT_HPP_

#include <vector>
#include <string>

#include "types.hpp"

namespace coconut {
namespace milk {
namespace fs {

class Path;

class Mount {
public:

	virtual ~Mount() = default;

	virtual std::vector<std::string> list(const Path& path) const = 0;

	virtual IStream open(const Path& path) const = 0;

	virtual OStream append(const Path& path) const = 0;

	virtual OStream overwrite(const Path& path) const = 0;

	virtual bool exists(const Path& path) const = 0;

	virtual bool readOnly() const noexcept = 0;

};

} // namespace fs
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_FILESYSTEM_MOUNT_HPP_ */
