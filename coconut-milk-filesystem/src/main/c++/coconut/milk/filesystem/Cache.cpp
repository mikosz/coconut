#include "Cache.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::filesystem;

std::shared_future<const RawData&> Cache::load(const Path& path, StreamOpener opener) volatile {
	auto lockedData = data_.lock();
	auto dataIt = lockedData->find(path);

	if (dataIt == lockedData->end()) {
		auto lockedTasks = tasks_.lock();
		auto taskIt = lockedTasks->find(path);

		if (taskIt == lockedTasks->end()) {
			auto future = std::async([this, &path, opener]() -> const RawData& { return loadStream(path, opener()); });
			taskIt = lockedTasks->emplace_hint(taskIt, path, future.share());
		}

		return taskIt->second;
	} else {
#pragma message("!!!!!!! DONT COMMIT THIS SHIT!")
		std::promise<const RawData&> promise;
		auto future = promise.get_future().share();
		promise.set_value(dataIt->second);

		return future;
	}
}

const RawData& Cache::loadStream(Path path, IStream is) volatile {
	RawData rawData;
	rawData.resize(3, '6');

	bool inserted;
	Data::iterator it;
	
	std::tie(it, inserted) = data_.lock()->emplace(path, std::move(rawData));
	tasks_.lock()->erase(path);

	assert(inserted);

	return it->second;
}
