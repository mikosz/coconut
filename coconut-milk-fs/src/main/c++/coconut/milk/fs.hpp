#ifndef _COCONUT_MILK_FS_HPP_
#define _COCONUT_MILK_FS_HPP_

#include "fs/Cache.hpp"
#include "fs/DirectoryMount.hpp"
#include "fs/Filesystem.hpp"
#include "fs/FilesystemContext.hpp"
#include "fs/Path.hpp"
#include "fs/types.hpp"

namespace coconut {
namespace milk {

using fs::Cache;
using fs::DirectoryMount;
using fs::Filesystem;
using fs::FilesystemContext;
using fs::AbsolutePath;
using fs::Path;
using fs::Byte;
using fs::RawData;
using fs::IStream;

} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_FS_HPP_ */
