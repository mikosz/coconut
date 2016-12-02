#include "Cache.hpp"

#include "Filesystem.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::filesystem;

std::shared_future<std::shared_ptr<const RawData>> Cache::load(const Path& path, StreamOpener opener) volatile {
	auto lockedData = data_.lock();
	auto dataIt = lockedData->find(path);

	if (dataIt == lockedData->end()) {
		auto future = std::async([this, &path, opener]() -> auto {
			return readRawData(path, opener());
		});
		dataIt = lockedData->emplace_hint(dataIt, path, future.share());
	}

	return dataIt->second;
}
