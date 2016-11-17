#ifndef _COCONUT_MILK_FILESYSTEM_CACHE_HPP_
#define _COCONUT_MILK_FILESYSTEM_CACHE_HPP_

#include <coconut-tools/concurrent/LockableInstance.hpp>

#include <unordered_map>
#include <future>
#include <functional>

#include "types.hpp"

namespace coconut {
namespace milk {
namespace filesystem {

class Cache {
public:

	using StreamOpener = std::function<IStream()>;

	std::shared_future<const RawData&> load(const Path& path, StreamOpener opener) volatile;

private:

	using Tasks = std::unordered_map<Path, std::shared_future<const RawData&>>;

	using Data = std::unordered_map<Path, const RawData>;

	coconut_tools::concurrent::LockableInstance<Tasks> tasks_;

	coconut_tools::concurrent::LockableInstance<Data> data_; // TODO: can we guarantee no copying?

	const RawData& loadStream(Path path, IStream is) volatile;

};

} // namespace filesystem
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_FILESYSTEM_CACHE_HPP_ */
