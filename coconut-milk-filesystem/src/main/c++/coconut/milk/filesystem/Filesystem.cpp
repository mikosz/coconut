#include "Filesystem.hpp"

#include <algorithm>
#include <iterator>

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::filesystem;

void Filesystem::mount(Path mountPoint, std::unique_ptr<MountRoot> mountRoot) {
	mounts_.emplace(std::move(mountPoint), std::move(mountRoot));
}

std::vector<std::string> Filesystem::list(const Path& path) const {
	std::vector<std::string> result;

	walk(path, [&result](const MountRoot& mountRoot, const Path& path) {
			auto files = mountRoot.list(path);
			if (result.empty()) {
				result.swap(files);
			} else {
				result.reserve(result.size() + files.size());
				std::move(files.begin(), files.end(), std::back_inserter(result));
			}
		}
		);

	return result;
}

IStream Filesystem::open(const Path& path) const {
	IStream is;
	walk(path, [&is](const MountRoot& mountRoot, const Path& path) {
			is = mountRoot.open(path);
		}
		);
	return is;
}

void Filesystem::walk(const Path& path, const WalkOp& walkOp) const {
	Path toMountPath = path;
	Path mountPath;

	for (;;) {
		auto mountIt = mounts_.find(toMountPath);

		if (mountIt != mounts_.end()) {
			walkOp(*mountIt->second, mountPath);
			return;
		}

		if (toMountPath.empty()) {
			throw "Abc"; // TODO
		} else {
			mountPath = toMountPath.filename() / mountPath;
			toMountPath.remove_filename();
		}
	}
}
