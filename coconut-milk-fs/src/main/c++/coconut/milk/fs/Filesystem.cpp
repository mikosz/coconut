#include "Filesystem.hpp"

#include <istream>
#include <algorithm>
#include <iterator>
#include <set>

#include <boost/range/adaptor/reversed.hpp>
#include <boost/range/adaptor/adjacent_filtered.hpp>

#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::fs;

namespace /* anonymous */ {

RawData readRawData(const AbsolutePath& path, std::istream& is) {
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

} // anonymous namespace

void Filesystem::mount(
	AbsolutePath mountPoint,
	std::unique_ptr<Mount> mount,
	PredecessorHidingPolicy predecessorHidingPolicy
	) {
	cache_.invalidateChildren(mountPoint);
	mounts_.emplace_back(std::move(mountPoint), std::move(mount), predecessorHidingPolicy);
}

std::vector<std::string> Filesystem::list(const AbsolutePath& path) const {
	auto filenameSet = std::set<std::string>();

	walk(
		path,
		[&filenameSet](const Mount& mount, const Path& pathInMount) {
			auto files = mount.list(pathInMount);
			std::move(files.begin(), files.end(), std::inserter(filenameSet, filenameSet.begin()));
		},
		true,
		true
		);

	auto result = std::vector<std::string>();
	result.reserve(filenameSet.size());
	std::move(filenameSet.begin(), filenameSet.end(), std::back_inserter(result));

	return result;
}

bool Filesystem::exists(const AbsolutePath& path) const {
	auto result = false;

	walk(
		path,
		[&result](const Mount& mount, const Path& pathInMount) {
			if (mount.exists(pathInMount)) {
				result = true;
			}
		},
		true,
		false
		);

	return result;
}

std::shared_future<SharedRawData> Filesystem::hint(const AbsolutePath& path) const {
	if (cache_.has(path)) {
		return cache_.get(path);
	} else {
		IStream is;

		walk(
			path,
			[&is](const Mount& mount, const Path& pathInMount) {
				is = mount.open(pathInMount);
			},
			false,
			true
			);

		return cache_.store(
			path,
			std::async([&path, is = std::move(is)]() -> SharedRawData {
				auto rawData = readRawData(path, *is);
				return std::make_shared<RawData>(std::move(rawData));
			})
			);
	}
}

SharedRawData Filesystem::load(const AbsolutePath& path) const {
	return hint(path).get();
}

IStream Filesystem::open(const AbsolutePath& path) const {
	const auto data = hint(path).get();

	auto source = boost::iostreams::array_source((char*)(data->data()), data->size());
	return std::make_unique<boost::iostreams::stream<decltype(source)>>(source);
}

OStream Filesystem::append(const AbsolutePath& path) const {
	cache_.invalidate(path);

	const auto directory = path.parent();
	const auto fileName = path.base();

	auto os = OStream();

	walk(
		directory,
		[&os, &fileName](const Mount& mount, const Path& directory) {
			os = mount.append(directory / fileName);
		},
		false,
		false
		);

	return os;
}

OStream Filesystem::overwrite(const AbsolutePath& path) const {
	cache_.invalidate(path);

	const auto directory = path.parent();
	const auto fileName = path.base();

	auto os = OStream();

	walk(
		directory,
		[&os, &fileName](const Mount& mount, const Path& directory) {
			os = mount.overwrite(directory / fileName);
		},
		false,
		false
		);

	return os;
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

void Filesystem::walk(
	const AbsolutePath& path,
	const WalkOp& walkOp,
	bool allowMultiple,
	bool requireExists
	) const {
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

	if (!found && requireExists) {
		throw InvalidPath("No such file or directory: " + path.string());
	}
}
