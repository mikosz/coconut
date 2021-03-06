#ifndef _COCONUT_MILK_FILESYSTEM_CACHE_HPP_
#define _COCONUT_MILK_FILESYSTEM_CACHE_HPP_

#include <future>
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

	bool has(const AbsolutePath& path) const;

	std::shared_future<SharedRawData> store(const AbsolutePath& path, std::future<SharedRawData> dataFuture);

	void invalidate(const AbsolutePath& path);

	void invalidateChildren(const AbsolutePath& path);

	std::shared_future<SharedRawData> get(const AbsolutePath& path);

private:

	using RemovalQueue = std::list<AbsolutePath>;

	struct Entry {
	
		std::shared_future<SharedRawData> dataFuture;

		RemovalQueue::iterator removalIt;
	
		Entry(std::shared_future<SharedRawData> dataFuture, RemovalQueue::iterator removalIt) :
			dataFuture(std::move(dataFuture)),
			removalIt(std::move(removalIt))
		{
		}

	};

	using CachedFiles = std::unordered_map<AbsolutePath, Entry>;

	CachedFiles cachedFiles_;

	RemovalQueue removalQueue_;

};

} // namespace fs
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_FILESYSTEM_CACHE_HPP_ */
