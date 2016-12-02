#ifndef _COCONUT_MILK_FILESYSTEM_DIRECTORYMOUNTROOT_HPP_
#define _COCONUT_MILK_FILESYSTEM_DIRECTORYMOUNTROOT_HPP_

#include "Types.hpp"
#include "MountRoot.hpp"

namespace coconut {
namespace milk {
namespace filesystem {

class DirectoryMountRoot : public MountRoot {
public:

	DirectoryMountRoot(Path root);

	std::vector<std::string> list(const Path& path) const override;

	IStream open(const Path& path) const override;

private:

	Path root_;

};

} // namespace filesystem
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_FILESYSTEM_DIRECTORYMOUNTROOT_HPP_ */
