#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include <sstream>

#include <coconut-tools/test-utils/test-utils.hpp>
#include <coconut-tools/test-utils/GMockFixture.hpp>

#include "coconut/milk/fs/Filesystem.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::fs;

using namespace std::string_literals;

namespace /* anonymous */ {

class MockMount : public Mount {
public:

	MOCK_CONST_METHOD1(list, std::vector<std::string> (const Path&));

	MOCK_CONST_METHOD1(openContents, std::string (const Path&));

	IStream open(const Path& path) const override {
		return std::make_unique<std::istringstream>(openContents(path));
	}

};

BOOST_FIXTURE_TEST_SUITE(MilkFsFilesystemTestSuite, coconut_tools::test_utils::GMockFixture);

BOOST_AUTO_TEST_CASE(ListsFilesInDirectory) {
	auto mountRootABC = std::make_unique<MockMount>();
	EXPECT_CALL(*mountRootABC, list(Path("")))
		.WillOnce(testing::Return(std::vector<std::string>{ "f1"s, "f2"s, "f3"s }));

	auto mountRootAB = std::make_unique<MockMount>();
	EXPECT_CALL(*mountRootAB, list(Path("")))
		.WillOnce(testing::Return(std::vector<std::string>{ "c"s }));

	Filesystem fs;
	fs.mount("/a/b"s, std::move(mountRootAB));
	fs.mount("/a/b/c"s, std::move(mountRootABC));

	BOOST_CHECK(fs.list("/a/b"s) == std::vector<std::string>{ "c"s });
	BOOST_CHECK(fs.list("/a/b/c"s) == (std::vector<std::string>{ "f1"s, "f2"s, "f3"s }));
}

BOOST_AUTO_TEST_CASE(OpensFiles) {
	auto mountRootABC = std::make_unique<MockMount>();
	EXPECT_CALL(*mountRootABC, openContents(Path("f")))
		.WillOnce(testing::Return("/a/b/c/f data\0"s));

	auto mountRootAB = std::make_unique<MockMount>();
	EXPECT_CALL(*mountRootAB, openContents(Path("f")))
		.WillOnce(testing::Return("/a/b/f data\0"s));

	Filesystem fs;
	fs.mount("/a/b"s, std::move(mountRootAB));
	fs.mount("/a/b/c"s, std::move(mountRootABC));

	BOOST_CHECK_EQUAL(
		std::string(std::istreambuf_iterator<char>(*fs.open("/a/b/f"s)), {}), "/a/b/f data\0"s);
	BOOST_CHECK_EQUAL(
		std::string(std::istreambuf_iterator<char>(*fs.open("/a/b/c/f"s)), {}), "/a/b/c/f data\0"s);
}

BOOST_AUTO_TEST_CASE(ThrowsIfNoMountInPath) {
	using namespace std::string_literals;

	Filesystem fs;

	BOOST_CHECK_THROW(fs.list("/"s), InvalidPath);
	BOOST_CHECK_THROW(fs.list("/dir"s), InvalidPath);
	BOOST_CHECK_THROW(fs.open("/"s), InvalidPath);
	BOOST_CHECK_THROW(fs.open("/dir"s), InvalidPath);
}

BOOST_AUTO_TEST_SUITE_END(/* MilkFsFilesystemTestSuite */);

} // anonymous namespace
