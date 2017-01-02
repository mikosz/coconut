#include "Path.hpp"

#include <algorithm>

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::fs;

namespace /* anonymous */ {

boost::filesystem::path unify(boost::filesystem::path physicalPath) {
	if (physicalPath.has_root_name()) {
		throw InvalidPath("Provided path \"" + physicalPath.string() +
			"\" is an absolute windows-style path, which makes no sense in this context");
	}

	physicalPath = physicalPath.lexically_normal();

	auto withDots = std::move(physicalPath);
	for (const auto& element : withDots) {
		if (element != ".") {
			physicalPath /= element;
		}
	}

	return physicalPath.generic();
}

} // anonymous namespace

Path::Path(boost::filesystem::path physicalPath) :
	unifiedPath_(unify(std::move(physicalPath)))
{
}

Path::Path(const char* pathString) :
	Path(boost::filesystem::path(pathString))
{
}

Path::Path(const std::string& pathString) :
	Path(boost::filesystem::path(pathString))
{
}

Path& Path::operator/=(const Path& tail) {
	if (tail.absolute() && !empty()) {
		throw InvalidPath("Cannot append absolute path \"" + tail.unifiedPath_.string() +
			"\" to nonempty path \"" + unifiedPath_.string() + '"');
	}
	unifiedPath_ /= tail.unifiedPath_;
	unifiedPath_ = unify(std::move(unifiedPath_));
	return *this;
}
