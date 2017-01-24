#include "Cache.hpp"

#include "Filesystem.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::fs;

auto Cache::load(const Filesystem& filesystem, const Path& path) -> std::shared_future<SharedRawData> {
	auto cachedIt = cachedFiles_.find(path);

	if (cachedIt == cachedFiles_.end()) {
		// TODO: filesystem must be valid until this operation completes, which is not great,
		// figure out something better
		auto dataFuture = std::async([&filesystem, path]() -> SharedRawData {
				auto rawData = readRawData(path, *filesystem.open(path));
				auto cachedData = std::make_shared<RawData>(std::move(rawData));
				return cachedData;
			});
		auto removalIt = removalQueue_.emplace(removalQueue_.end(), path);
		bool inserted;
		Entry entry(dataFuture.share(), removalIt);
		std::tie(cachedIt, inserted) = cachedFiles_.emplace(path, std::move(entry));

		assert(inserted);
	}

	return cachedIt->second.dataFuture;
}
