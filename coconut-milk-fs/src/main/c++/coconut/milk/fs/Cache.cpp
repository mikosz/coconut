#include "Cache.hpp"

#include "Filesystem.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::fs;

std::shared_future<std::shared_ptr<const RawData>> Cache::load(const Path& path, const Filesystem& fs) volatile {
	auto lockedData = data_.lock();
	auto dataIt = lockedData->find(path);

	if (dataIt == lockedData->end()) {
		auto future = std::async([&path, &fs]() -> auto {
			auto rawData = readRawData(path, *fs.open(path));
			return std::make_shared<const RawData>(std::move(rawData));
		});
		dataIt = lockedData->emplace_hint(dataIt, path, future.share());
	}

	return dataIt->second;
}
