#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include <algorithm>

#include <boost/filesystem.hpp>

#include <coconut-tools/test-utils/test-utils.hpp>

#include "coconut/milk/fs/DirectoryMount.hpp"
#include "coconut/milk/fs/Path.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::fs;

using namespace std::string_literals;

namespace /* anonymous */ {

std::vector<std::string> sort(std::vector<std::string>&& v) {
	std::sort(v.begin(), v.end());
	return std::move(v);
}

BOOST_FIXTURE_TEST_SUITE(MilkFsDirectoryMountTestSuite, coconut_tools::test_utils::ResourcesDirFixture);

BOOST_AUTO_TEST_CASE(ListsFilesInRootDirectory) {
	DirectoryMount mount(resourcesDir());

	boost::filesystem::create_directories(resourcesDir() / "subdir");
	coconut_tools::test_utils::writeToFile(resourcesDir() / "f1", ""s);
	coconut_tools::test_utils::writeToFile(resourcesDir() / "f2", ""s);
	
	BOOST_CHECK(sort(mount.list("")) == (std::vector<std::string>{"f1"s, "f2"s, "subdir"s}));
	BOOST_CHECK(sort(mount.list("/")) == (std::vector<std::string>{"f1"s, "f2"s, "subdir"s}));
}

BOOST_AUTO_TEST_CASE(ListsFilesInSubdirectory) {
	DirectoryMount mount(resourcesDir());

	boost::filesystem::create_directories(resourcesDir() / "dir/subdir");
	coconut_tools::test_utils::writeToFile(resourcesDir() / "dir" / "f1", ""s);
	coconut_tools::test_utils::writeToFile(resourcesDir() / "dir" / "f2", ""s);

	BOOST_CHECK(sort(mount.list("dir")) == (std::vector<std::string>{"f1"s, "f2"s, "subdir"s}));
	BOOST_CHECK(sort(mount.list("/dir")) == (std::vector<std::string>{"f1"s, "f2"s, "subdir"s}));
}

BOOST_AUTO_TEST_CASE(ReturnsEmptyVectorForEmptyDirectories) {
	DirectoryMount mount(resourcesDir());
	BOOST_CHECK(mount.list("/").empty());
}

BOOST_AUTO_TEST_CASE(ThrowsOnInvalidPath) {
	coconut_tools::test_utils::writeToFile(resourcesDir() / "is-file", ""s);
	boost::filesystem::create_directory(resourcesDir() / "directory");

	BOOST_CHECK_THROW(DirectoryMount(resourcesDir() / "doesnt-exist"), InvalidPath);
	BOOST_CHECK_THROW(DirectoryMount(resourcesDir() / "is-file"), InvalidPath);

	DirectoryMount mount(resourcesDir());

	BOOST_CHECK_THROW(mount.list("doesnt-exist"), InvalidPath);
	BOOST_CHECK_THROW(mount.list("is-file"), InvalidPath);
	BOOST_CHECK_THROW(mount.open("directory"), InvalidPath);
}

BOOST_AUTO_TEST_SUITE_END(/* MilkFsDirectoryMountTestSuite */);

} // anonymous namespace
