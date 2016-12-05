#include "DirectoryMount.hpp"

#include <algorithm>
#include <iterator>
#include <fstream>

#include <boost/filesystem.hpp>

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::fs;

using namespace std::string_literals;

DirectoryMount::DirectoryMount(Path root) :
	root_(std::move(root))
{
	if (!boost::filesystem::is_directory(root)) {
		throw InvalidPath("Not a path to an existing directory: \""s + root.string() + '"');
	}
}

std::vector<std::string> DirectoryMount::list(const Path& path) const {
	const auto effectivePath = root_ / path;

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
	return std::make_unique<std::ifstream>((root_ / path).c_str());
}
