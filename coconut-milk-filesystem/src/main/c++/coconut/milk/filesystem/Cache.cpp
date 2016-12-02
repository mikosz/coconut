#include "Cache.hpp"

#include "Filesystem.hpp"
#include "operations.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::filesystem;

std::shared_future<std::shared_ptr<const RawData>> Cache::load(const Path& path, const Filesystem& filesystem) volatile {
	auto lockedData = data_.lock();
	auto dataIt = lockedData->find(path);

	if (dataIt == lockedData->end()) {
		auto future = std::async([&path, &filesystem]() -> auto {
			auto rawData = readRawData(path, *filesystem.open(path));
			return std::make_shared<const RawData>(std::move(rawData));
		});
		dataIt = lockedData->emplace_hint(dataIt, path, future.share());
	}

	return dataIt->second;
}
