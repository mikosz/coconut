#include "Path.hpp"

#include <algorithm>

#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

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

AbsolutePath::AbsolutePath(boost::filesystem::path physicalPath) :
	Path(physicalPath)
{
	if (relative()) {
		throw InvalidPath("Absolute path required, but relative provided: \"" + physicalPath.string() + "\"");
	}
}

AbsolutePath::AbsolutePath(const Path& path) :
	AbsolutePath(path.physicalPath())
{
}

AbsolutePath::AbsolutePath(const char* pathString) :
	AbsolutePath(boost::filesystem::path(pathString))
{
}

AbsolutePath::AbsolutePath(const std::string& pathString) :
	AbsolutePath(boost::filesystem::path(pathString))
{
}

boost::optional<Path> AbsolutePath::relativeTo(const AbsolutePath& parent) const {
	const auto isSlash = boost::is_any_of("/");

	auto parentSplit = std::vector<std::string>();
	boost::split(parentSplit, parent.physicalPath().string(), isSlash, boost::algorithm::token_compress_on);
	parentSplit.erase(std::remove(parentSplit.begin(), parentSplit.end(), ""), parentSplit.end());

	auto thisSplit = std::vector<std::string>();
	boost::split(thisSplit, physicalPath().string(), isSlash, boost::algorithm::token_compress_on);
	thisSplit.erase(std::remove(thisSplit.begin(), thisSplit.end(), ""), thisSplit.end());

	if (parentSplit.size() <= thisSplit.size()) {
		if (std::equal(parentSplit.begin(), parentSplit.end(), thisSplit.begin())) {
			const auto subPath = boost::join(
				std::vector<std::string>(thisSplit.begin() + parentSplit.size(), thisSplit.end()), "/");
			return Path(subPath);
		}
	}
	
	return boost::optional<Path>();
}

void fs::serialise(coconut_tools::Serialiser& serialiser, const AbsolutePath& path) {
	serialiser(coconut_tools::Serialiser::Label("path"), path.string());
}

void fs::serialise(coconut_tools::Deserialiser& deserialiser, AbsolutePath& path) {
	auto string = std::string();
	deserialiser(coconut_tools::Deserialiser::Label("path"), string);
	path = string;
}
