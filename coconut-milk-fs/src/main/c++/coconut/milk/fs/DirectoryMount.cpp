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

namespace /* anonymous */ {

template <class T>
std::unique_ptr<T> openStream(
	const boost::filesystem::path& root,
	const Path& path,
	unsigned int flags,
	bool requireExists
	) {
	const auto effectivePath = root / path.physicalPath();

	const auto exists = boost::filesystem::exists(effectivePath);

	if (exists) {
		if (!boost::filesystem::is_regular_file(effectivePath)) {
			throw InvalidPath("Not a path to a regular file: \"" + effectivePath.string() + '"');
		}
	} else if (requireExists) {
		throw InvalidPath("File does not exist: \"" + effectivePath.string() + '"');
	}

	return std::make_unique<T>(effectivePath.c_str(), flags | std::ios::binary);
}

} // anonymous namespace

DirectoryMount::DirectoryMount(boost::filesystem::path root, bool readOnly) :
	root_(std::move(root)),
	readOnly_(readOnly)
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
	return openStream<std::ifstream>(root_, path, std::ios::in, true);
}

OStream DirectoryMount::append(const Path& path) const {
	return openStream<std::ofstream>(root_, path, std::ios::app, false);
}

OStream DirectoryMount::overwrite(const Path& path) const {
	return openStream<std::ofstream>(root_, path, std::ios::out, false);
}

bool DirectoryMount::exists(const Path& path) const {
	const auto effectivePath = root_ / path.physicalPath();

	return boost::filesystem::exists(effectivePath);
}
