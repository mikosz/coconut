#include "Filesystem.hpp"

#include <istream>
#include <algorithm>
#include <iterator>
#include <set>

#include <boost/range/adaptor/reversed.hpp>
#include <boost/range/adaptor/adjacent_filtered.hpp>

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::fs;

void Filesystem::mount(
	AbsolutePath mountPoint,
	std::unique_ptr<Mount> mount,
	PredecessorHidingPolicy predecessorHidingPolicy
	) {
	mounts_.emplace_back(std::move(mountPoint), std::move(mount), predecessorHidingPolicy);
}

std::vector<std::string> Filesystem::list(const AbsolutePath& path) const {
	auto filenameSet = std::set<std::string>();

	walk(
		path,
		[&filenameSet](const Mount& mount, const Path& path) {
			auto files = mount.list(path);
			std::move(files.begin(), files.end(), std::inserter(filenameSet, filenameSet.begin()));
		},
		true
		);

	auto result = std::vector<std::string>();
	result.reserve(filenameSet.size());
	std::move(filenameSet.begin(), filenameSet.end(), std::back_inserter(result));

	return result;
}

IStream Filesystem::open(const AbsolutePath& path) const {
	auto is = IStream();

	walk(
		path,
		[&is](const Mount& mount, const Path& path) {
			is = mount.open(path);
		},
		false
		);

	return is;
}

Filesystem::MountEntry::MountEntry(
	AbsolutePath mountPoint,
	std::unique_ptr<Mount> mount,
	PredecessorHidingPolicy predecessorHidingPolicy
	) :
	mountPoint(std::move(mountPoint)),
	mount(std::move(mount)),
	predecessorHidingPolicy(predecessorHidingPolicy)
{
}

void Filesystem::walk(const AbsolutePath& path, const WalkOp& walkOp, bool allowMultiple) const {
	auto found = false;

	for (const auto& mountEntry : (mounts_ | boost::adaptors::reversed)) {
		const auto subPath = path.relativeTo(mountEntry.mountPoint);
		if (subPath) {
			if (mountEntry.mount->exists(*subPath)) {
				walkOp(*mountEntry.mount, *subPath);
				found = true;
				if (!allowMultiple) {
					return;
				}
			}
			if (mountEntry.predecessorHidingPolicy == PredecessorHidingPolicy::HIDE) {
				break;
			}
		}
	}

	if (!found) {
		throw InvalidPath("No such file or directory: " + path.string());
	}
}

RawData coconut::milk::fs::readRawData(const AbsolutePath& path, std::istream& is) {
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
