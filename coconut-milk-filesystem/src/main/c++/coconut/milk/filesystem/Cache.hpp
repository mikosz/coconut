#ifndef _COCONUT_MILK_FILESYSTEM_CACHE_HPP_
#define _COCONUT_MILK_FILESYSTEM_CACHE_HPP_

#include <coconut-tools/concurrent/LockableInstance.hpp>

#include <unordered_map>
#include <future>
#include <functional>
#include <memory>

#include "types.hpp"

namespace coconut {
namespace milk {
namespace filesystem {

class Cache {
public:

	using FutureRawData = std::shared_future<std::shared_ptr<const RawData>>;

	using StreamOpener = std::function<IStream()>;

	FutureRawData load(const Path& path, StreamOpener opener) volatile;

private:

	using Data = std::unordered_map<Path, FutureRawData>;

	coconut_tools::concurrent::LockableInstance<Data> data_;

};

} // namespace filesystem
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_FILESYSTEM_CACHE_HPP_ */
