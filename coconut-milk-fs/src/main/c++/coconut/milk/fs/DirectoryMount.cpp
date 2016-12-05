#include "DirectoryMount.hpp"

#include <algorithm>
#include <iterator>
#include <fstream>

#include <boost/filesystem.hpp>

#include "operations.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::fs;

DirectoryMount::DirectoryMount(Path root) :
	root_(std::move(root))
{
	if (boost::filesystem::exists(root) && !boost::filesystem::is_directory(root)) {
		throw "abc"; // TODO
	}
}

std::vector<std::string> DirectoryMount::list(const Path& path) const {
	std::vector<std::string> contents;
	std::transform(
		boost::filesystem::directory_iterator(path),
		boost::filesystem::directory_iterator(),
		std::back_inserter(contents),
		[](const auto& entry) { return entry.path().string(); }
		);
	return contents;
}

IStream DirectoryMount::open(const Path& path) const {
	return std::make_unique<std::ifstream>(path.c_str());
}
