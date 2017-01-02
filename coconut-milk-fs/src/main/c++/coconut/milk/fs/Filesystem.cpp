#include "Filesystem.hpp"

#include <istream>
#include <algorithm>
#include <iterator>

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::fs;

void Filesystem::mount(Path mountPoint, std::unique_ptr<Mount> mountRoot) {
	mounts_.emplace(std::move(mountPoint), std::move(mountRoot));
}

std::vector<std::string> Filesystem::list(const Path& path) const {
	std::vector<std::string> result;

	walk(path, [&result](const Mount& mountRoot, const Path& path) {
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
	walk(path, [&is](const Mount& mountRoot, const Path& path) {
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
			throw InvalidPath("No such file or directory: " + path.string());
		} else {
			mountPath = toMountPath / mountPath;
			toMountPath = toMountPath.parent();
		}
	}
}

RawData coconut::milk::fs::readRawData(const Path& path, std::istream& is) {
	try {
		is.exceptions(std::ios::badbit);

		is.seekg(0u, std::ios::end);
		RawData rawData(static_cast<size_t>(is.tellg()));
		is.seekg(0u, std::ios::beg);
		is.read(reinterpret_cast<char*>(rawData.data()), rawData.size());

		return rawData;
	}
	catch (const std::runtime_error& e) {
		throw FailedToReadData(path, e);
	}
}
