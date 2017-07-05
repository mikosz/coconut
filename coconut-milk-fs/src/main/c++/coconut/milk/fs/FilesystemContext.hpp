#ifndef _COCONUT_MILK_FILESYSTEM_FILESYSTEMCONTEXT_HPP_
#define _COCONUT_MILK_FILESYSTEM_FILESYSTEMCONTEXT_HPP_

#include <memory>
#include <vector>

#include "Filesystem.hpp"
#include "Path.hpp"
#include "types.hpp"

namespace coconut {
namespace milk {
namespace fs {

class FilesystemContext {
public:

	FilesystemContext(std::shared_ptr<Filesystem> filesystem);

	FilesystemContext createContext(const Path& workingDirectory) const; // TODO: API

	void changeWorkingDirectory(const Path& path);

	void mount(
		Path mountPoint,
		std::unique_ptr<Mount> mountRoot,
		Filesystem::PredecessorHidingPolicy predecessorHidingPolicy
		);

	std::vector<std::string> list(const Path& path) const;

	bool exists(const Path& path) const;

	std::shared_future<SharedRawData> hint(const Path& path) const;

	SharedRawData load(const Path& path) const;

	IStream open(const Path& path) const;

	OStream append(const Path& path) const;

	OStream overwrite(const Path& path) const;

	const AbsolutePath& currentWorkingDirectory() const {
		return currentWorkingDirectory_;
	}

	AbsolutePath makeAbsolute(const Path& path) const;

private:

	std::shared_ptr<Filesystem> filesystem_;
	
	AbsolutePath currentWorkingDirectory_;

};

} // namespace fs
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_FILESYSTEM_FILESYSTEMCONTEXT_HPP_ */
