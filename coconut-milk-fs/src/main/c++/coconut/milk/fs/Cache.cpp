#include "Cache.hpp"

#include <coconut-tools/exceptions/LogicError.hpp>

#include "Filesystem.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::fs;

bool Cache::has(const AbsolutePath& path) const {
	return cachedFiles_.count(path) != 0;
}

std::shared_future<SharedRawData> Cache::store(
	const AbsolutePath& path, std::future<SharedRawData> dataFuture)
{
	if (has(path)) {
		throw coconut_tools::exceptions::LogicError(
			"Attempted to store data in cache for an already existing entry");
	}

	auto removalIt = removalQueue_.emplace(removalQueue_.end(), path);
	Entry entry(dataFuture.share(), removalIt);

	auto inserted = false;
	auto cachedIt = cachedFiles_.end();
	std::tie(cachedIt, inserted) = cachedFiles_.emplace(path, std::move(entry));

	assert(inserted);
	
	return cachedIt->second.dataFuture;
}

void Cache::invalidate(const AbsolutePath& path) {
	auto cachedIt = cachedFiles_.find(path);
	if (cachedIt != cachedFiles_.end()) {
		removalQueue_.erase(cachedIt->second.removalIt);
		cachedFiles_.erase(cachedIt);
	}
}

void Cache::invalidateChildren(const AbsolutePath& path) {
	auto cachedIt = cachedFiles_.begin();
	const auto cachedEnd = cachedFiles_.end();

	while (cachedIt != cachedEnd) {
		if (cachedIt->first.relativeTo(path)) {
			removalQueue_.erase(cachedIt->second.removalIt);
			cachedFiles_.erase(cachedIt++);
		} else {
			++cachedIt;
		}
	}
}

std::shared_future<SharedRawData> Cache::get(const AbsolutePath& path) {
	if (!has(path)) {
		throw coconut_tools::exceptions::LogicError("Attempted to retrieve non-existing data from cache");
	}

	return cachedFiles_.at(path).dataFuture;
}
