#ifndef _COCONUT_MILK_FILESYSTEM_FILESYSTEM_HPP_
#define _COCONUT_MILK_FILESYSTEM_FILESYSTEM_HPP_

#include <vector>
#include <string>
#include <iosfwd>
#include <memory>
#include <functional>
#include <unordered_map>

#include "MountRoot.hpp"
#include "types.hpp"

namespace coconut {
namespace milk {
namespace filesystem {

class Filesystem {
public:

	void mount(Path mountPoint, std::unique_ptr<MountRoot> mountRoot);

	std::vector<std::string> list(const Path& path) const;

	IStream open(const Path& path) const;

private:

	using WalkOp = std::function<void (const MountRoot&, const Path&)>;

	std::unordered_map<Path, std::unique_ptr<MountRoot>> mounts_;

	void walk(const Path& path, const WalkOp& walkOp) const;

};

} // namespace filesystem
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_FILESYSTEM_FILESYSTEM_HPP_ */
