#ifndef _COCONUT_MILK_FILESYSTEM_FILESYSTEMCONTEXT_HPP_
#define _COCONUT_MILK_FILESYSTEM_FILESYSTEMCONTEXT_HPP_

#include <memory>
#include <vector>

#include "Cache.hpp"
#include "Filesystem.hpp"
#include "Path.hpp"
#include "types.hpp"

namespace coconut {
namespace milk {
namespace fs {

class Mount;

class FilesystemContext {
public:

	FilesystemContext(std::shared_ptr<Filesystem> filesystem, std::shared_ptr<Cache> cache);

	void changeWorkingDirectory(const Path& path);

	void mount(
		Path mountPoint,
		std::unique_ptr<Mount> mountRoot,
		Filesystem::PredecessorHidingPolicy predecessorHidingPolicy
		);

	std::vector<std::string> list(const Path& path) const;

	std::shared_future<Cache::CachedData> load(const Path& path) const;

private:

	std::shared_ptr<Filesystem> filesystem_;
	
	// TODO: not sure about cache being here
	std::shared_ptr<Cache> cache_;

	AbsolutePath currentWorkingDirectory_;

	AbsolutePath toAbsolutePath(const Path& path) const;

};

} // namespace fs
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_FILESYSTEM_FILESYSTEMCONTEXT_HPP_ */
