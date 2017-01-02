#ifndef _COCONUT_MILK_FILESYSTEM_CACHE_HPP_
#define _COCONUT_MILK_FILESYSTEM_CACHE_HPP_

#include <future>
#include <mutex>
#include <memory>
#include <list>
#include <unordered_map>

#include "Path.hpp"
#include "types.hpp"

namespace coconut {
namespace milk {
namespace fs {

class Filesystem;

class Cache {
public:

	using CachedData = std::shared_ptr<const RawData>;

	std::shared_future<CachedData> load(const Filesystem& filesystem, const Path& path);

private:

	using RemovalQueue = std::list<Path>;

	struct Entry {
	
		std::shared_future<CachedData> dataFuture;

		RemovalQueue::iterator removalIt;
	
		Entry(std::shared_future<CachedData> dataFuture, RemovalQueue::iterator removalIt) :
			dataFuture(std::move(dataFuture)),
			removalIt(std::move(removalIt))
		{
		}

	};

	using CachedFiles = std::unordered_map<Path, Entry>;

	CachedFiles cachedFiles_;

	RemovalQueue removalQueue_;

};

} // namespace fs
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_FILESYSTEM_CACHE_HPP_ */
