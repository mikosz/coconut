#include "FilesystemContext.hpp"

#include <cassert>

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::fs;

FilesystemContext::FilesystemContext(std::shared_ptr<Filesystem> filesystem, std::shared_ptr<Cache> cache) :
	filesystem_(std::move(filesystem)),
	cache_(std::move(cache)),
	currentWorkingDirectory_("/")
{
	assert(filesystem_);
	assert(cache_);
}

void FilesystemContext::changeWorkingDirectory(const Path& path) {
	currentWorkingDirectory_ = toAbsolutePath(path);
}

void FilesystemContext::mount(
	Path mountPoint,
	std::unique_ptr<Mount> mountRoot,
	Filesystem::PredecessorHidingPolicy predecessorHidingPolicy
	) {
	filesystem_->mount(toAbsolutePath(mountPoint), std::move(mountRoot), predecessorHidingPolicy);
}

std::vector<std::string> FilesystemContext::list(const Path& path) const {
	return filesystem_->list(toAbsolutePath(path));
}

std::shared_future<Cache::CachedData> FilesystemContext::load(const Path& path) const {
	return cache_->load(*filesystem_, toAbsolutePath(path));
}

AbsolutePath FilesystemContext::toAbsolutePath(const Path& path) const {
	if (path.absolute()) {
		return path;
	} else {
		return currentWorkingDirectory_ / path;
	}
}