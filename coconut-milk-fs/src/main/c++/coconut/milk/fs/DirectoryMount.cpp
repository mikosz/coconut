#include "DirectoryMount.hpp"

#include <algorithm>
#include <iterator>
#include <fstream>

#include <boost/filesystem.hpp>

#include "Path.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::fs;

using namespace std::string_literals;

DirectoryMount::DirectoryMount(boost::filesystem::path root) :
	root_(std::move(root))
{
	if (!boost::filesystem::is_directory(root_)) {
		throw InvalidPath("Not a path to an existing directory: \""s + root_.string() + '"');
	}
}

std::vector<std::string> DirectoryMount::list(const Path& path) const {
	const auto effectivePath = root_ / path.physicalPath();

	if (!boost::filesystem::is_directory(effectivePath)) {
		throw InvalidPath("Not a path to an existing directory: \""s + effectivePath.string() + '"');
	}
	std::vector<std::string> contents;
	std::transform(
		boost::filesystem::directory_iterator(effectivePath),
		boost::filesystem::directory_iterator(),
		std::back_inserter(contents),
		[](const auto& entry) { return entry.path().filename().string(); }
		);
	return contents;
}

IStream DirectoryMount::open(const Path& path) const {
	const auto effectivePath = root_ / path.physicalPath();

	if (!boost::filesystem::is_regular_file(effectivePath)) {
		throw InvalidPath("Not a path to a regular file: \"" + effectivePath.string() + '"');
	}

	return std::make_unique<std::ifstream>(effectivePath.c_str());
}

bool DirectoryMount::exists(const Path& path) const {
	const auto effectivePath = root_ / path.physicalPath();

	return boost::filesystem::exists(effectivePath);
}
