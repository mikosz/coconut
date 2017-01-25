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
	currentWorkingDirectory_ = toAbsolutePath(path);
}

void FilesystemContext::mount(
	Path mountPoint,
	std::unique_ptr<Mount> mountRoot,
	Filesystem::PredecessorHidingPolicy predecessorHidingPolicy
	)
{
	filesystem_->mount(toAbsolutePath(mountPoint), std::move(mountRoot), predecessorHidingPolicy);
}

std::vector<std::string> FilesystemContext::list(const Path& path) const {
	return filesystem_->list(toAbsolutePath(path));
}

bool FilesystemContext::exists(const Path& path) const {
	return filesystem_->exists(toAbsolutePath(path));
}

std::shared_future<SharedRawData> FilesystemContext::hint(const Path& path) const {
	return filesystem_->hint(toAbsolutePath(path));
}

SharedRawData FilesystemContext::load(const Path& path) const {
	return filesystem_->load(toAbsolutePath(path));
}

IStream FilesystemContext::open(const Path& path) const {
	return filesystem_->open(toAbsolutePath(path));
}

OStream FilesystemContext::append(const Path& path) const {
	return filesystem_->append(toAbsolutePath(path));
}

OStream FilesystemContext::overwrite(const Path& path) const {
	return filesystem_->overwrite(toAbsolutePath(path));
}

AbsolutePath FilesystemContext::toAbsolutePath(const Path& path) const {
	if (path.absolute()) {
		return path;
	} else {
		return currentWorkingDirectory_ / path;
	}
}
