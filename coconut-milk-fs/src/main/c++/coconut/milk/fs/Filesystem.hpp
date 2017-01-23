#ifndef _COCONUT_MILK_FS_FILESYSTEM_HPP_
#define _COCONUT_MILK_FS_FILESYSTEM_HPP_

#include <vector>
#include <string>
#include <iosfwd>
#include <memory>
#include <functional>

#include <coconut-tools/exceptions/RuntimeError.hpp>
#include <coconut-tools/enum.hpp>

#include "Mount.hpp"
#include "Path.hpp"
#include "types.hpp"

namespace coconut {
namespace milk {
namespace fs {

class Filesystem {
public:

	CCN_MEMBER_ENUM(
		PredecessorHidingPolicy,
		(HIDE)
		(ADD)
		);

	using Filenames = std::vector<std::string>;

	void mount(
		AbsolutePath mountPoint,
		std::unique_ptr<Mount> mount,
		PredecessorHidingPolicy predecessorHidingPolicy
		);

	Filenames list(const AbsolutePath& path) const;

	bool exists(const AbsolutePath& path) const;

	IStream open(const AbsolutePath& path) const;

	OStream append(const AbsolutePath& path) const;

	OStream overwrite(const AbsolutePath& path) const;

private:

	struct MountEntry {

		AbsolutePath mountPoint;

		std::unique_ptr<Mount> mount;

		PredecessorHidingPolicy predecessorHidingPolicy;

		MountEntry(
			AbsolutePath mountPoint,
			std::unique_ptr<Mount> mount,
			PredecessorHidingPolicy predecessorHidingPolicy
			);

	};

	using WalkOp = std::function<void (const Mount&, const Path&)>;

	using Mounts = std::vector<MountEntry>;

	Mounts mounts_;

	void walk(const AbsolutePath& path, const WalkOp& walkOp, bool allowMultiple) const;

};

class FailedToReadData : public coconut_tools::exceptions::RuntimeError {
public:

	FailedToReadData(const AbsolutePath& path, const std::runtime_error& cause) :
		coconut_tools::exceptions::RuntimeError("Failed to read data from \"" + path.string() + '\"', cause)
	{
	}

};

RawData readRawData(const AbsolutePath& path, std::istream& is);

} // namespace fs
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_FS_FILESYSTEM_HPP_ */
