#ifndef _COCONUT_MILK_FILESYSTEM_DIRECTORYMOUNT_HPP_
#define _COCONUT_MILK_FILESYSTEM_DIRECTORYMOUNT_HPP_

#include <boost/filesystem/path.hpp>

#include "Mount.hpp"

namespace coconut {
namespace milk {
namespace fs {

class Path;

class DirectoryMount : public Mount {
public:

	DirectoryMount(boost::filesystem::path root);

	std::vector<std::string> list(const Path& path) const override;

	IStream open(const Path& path) const override;

	bool exists(const Path& path) const override;

private:

	boost::filesystem::path root_;

};

} // namespace fs
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_FILESYSTEM_DIRECTORYMOUNT_HPP_ */
