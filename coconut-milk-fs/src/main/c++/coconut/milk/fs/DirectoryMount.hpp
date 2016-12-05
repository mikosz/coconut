#ifndef _COCONUT_MILK_FILESYSTEM_DIRECTORYMOUNT_HPP_
#define _COCONUT_MILK_FILESYSTEM_DIRECTORYMOUNT_HPP_

#include "Types.hpp"
#include "Mount.hpp"

namespace coconut {
namespace milk {
namespace fs {

class DirectoryMount : public Mount {
public:

	DirectoryMount(Path root);

	std::vector<std::string> list(const Path& path) const override;

	IStream open(const Path& path) const override;

private:

	Path root_;

};

} // namespace fs
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_FILESYSTEM_DIRECTORYMOUNT_HPP_ */
