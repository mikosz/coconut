#include "FilesystemContext.hpp"

#include <cassert>

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::fs;

FilesystemContext::FilesystemContext(std::shared_ptr<Filesystem> filesystem) :
	filesystem_(std::move(filesystem)),
	currentWorkingDirectory_("/")
{
	assert(filesystem_);
}

void FilesystemContext::changeWorkingDirectory(const Path& path) {
	currentWorkingDirectory_ = makeAbsolute(path);
}

void FilesystemContext::mount(
	Path mountPoint,
	std::unique_ptr<Mount> mountRoot,
	Filesystem::PredecessorHidingPolicy predecessorHidingPolicy
	)
{
	filesystem_->mount(makeAbsolute(mountPoint), std::move(mountRoot), predecessorHidingPolicy);
}

std::vector<std::string> FilesystemContext::list(const Path& path) const {
	return filesystem_->list(makeAbsolute(path));
}

bool FilesystemContext::exists(const Path& path) const {
	return filesystem_->exists(makeAbsolute(path));
}

std::shared_future<SharedRawData> FilesystemContext::hint(const Path& path) const {
	return filesystem_->hint(makeAbsolute(path));
}

SharedRawData FilesystemContext::load(const Path& path) const {
	return filesystem_->load(makeAbsolute(path));
}

IStream FilesystemContext::open(const Path& path) const {
	return filesystem_->open(makeAbsolute(path));
}

OStream FilesystemContext::append(const Path& path) const {
	return filesystem_->append(makeAbsolute(path));
}

OStream FilesystemContext::overwrite(const Path& path) const {
	return filesystem_->overwrite(makeAbsolute(path));
}

AbsolutePath FilesystemContext::makeAbsolute(const Path& path) const {
	if (path.absolute()) {
		return path;
	} else {
		return currentWorkingDirectory_ / path;
	}
}
