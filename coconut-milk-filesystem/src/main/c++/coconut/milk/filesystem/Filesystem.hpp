#ifndef _COCONUT_MILK_FILESYSTEM_FILESYSTEM_HPP_
#define _COCONUT_MILK_FILESYSTEM_FILESYSTEM_HPP_

#include <vector>

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

} // namespace filesystem
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_FILESYSTEM_FILESYSTEM_HPP_ */
