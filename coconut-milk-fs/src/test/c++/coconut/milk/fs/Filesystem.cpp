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

	MOCK_CONST_METHOD1(exists, bool (const Path&));

	IStream open(const Path& path) const override {
		return std::make_unique<std::istringstream>(openContents(path));
	}

	OStream append(const Path&) const override {
		return OStream();
	}

	OStream overwrite(const Path&) const override {
		return OStream();
	}

	bool readOnly() const noexcept override {
		return true;
	}

};

BOOST_FIXTURE_TEST_SUITE(MilkFsFilesystemTestSuite, coconut_tools::test_utils::GMockFixture);

BOOST_AUTO_TEST_CASE(ListsFiles) {
	/*
	 * - Mounted at /:
	 * - dir/
	 * -- f
	 * - f
	 */

	auto mount = std::make_unique<MockMount>();
	EXPECT_CALL(*mount, list(Path("")))
		.WillOnce(testing::Return(std::vector<std::string>{ "dir"s, "f"s }));
	EXPECT_CALL(*mount, list(Path("dir")))
		.WillOnce(testing::Return(std::vector<std::string>{ "f"s }));
	EXPECT_CALL(*mount, exists(Path(""))).WillOnce(testing::Return(true));
	EXPECT_CALL(*mount, exists(Path("dir"))).WillOnce(testing::Return(true));

	Filesystem fs;
	fs.mount("/"s, std::move(mount), Filesystem::PredecessorHidingPolicy::HIDE);

	BOOST_CHECK(fs.list("/"s) == (std::vector<std::string>{ "dir"s, "f"s }));
	BOOST_CHECK(fs.list("/dir"s) == std::vector<std::string>{ "f"s });
}

BOOST_AUTO_TEST_CASE(OpensFiles) {
	/*
	 * - Mounted at /:
	 * - f
	 */

	auto mount = std::make_unique<MockMount>();
	const auto fileContents = "/f data\0"s;

	EXPECT_CALL(*mount, exists(Path("f"))).WillOnce(testing::Return(true));
	EXPECT_CALL(*mount, openContents(Path("f"))).WillOnce(testing::Return(fileContents));

	Filesystem fs;
	fs.mount("/"s, std::move(mount), Filesystem::PredecessorHidingPolicy::HIDE);

	BOOST_CHECK_EQUAL(
		std::string(std::istreambuf_iterator<char>(*fs.open("/f"s)), {}), fileContents);
}

BOOST_AUTO_TEST_CASE(FilesHiddenWhenAnotherMountHides) {
	/*
	 * - Mounted at /:
	 * - f
	 * - f1
	 * - Mounted at /:
	 * - f
	 * - f2
	 * - Mounted at /dir:
	 * - f3
	 */

	auto mountRoot = std::make_unique<MockMount>();
	EXPECT_CALL(*mountRoot, list(Path("")))
		.WillRepeatedly(testing::Return(std::vector<std::string>{ "f"s, "f1"s }));
	EXPECT_CALL(*mountRoot, exists(Path(""))).WillRepeatedly(testing::Return(true));

	Filesystem fs;
	fs.mount("/"s, std::move(mountRoot), Filesystem::PredecessorHidingPolicy::HIDE);

	auto mountRootOverride = std::make_unique<MockMount>();
	EXPECT_CALL(*mountRootOverride, list(Path("")))
		.WillRepeatedly(testing::Return(std::vector<std::string>{ "f"s, "f2"s }));
	EXPECT_CALL(*mountRootOverride, exists(Path(""))).WillRepeatedly(testing::Return(true));
	EXPECT_CALL(*mountRootOverride, openContents(Path("f"))).WillOnce(testing::Return("root override\0"s));
	EXPECT_CALL(*mountRootOverride, exists(Path("f"))).WillOnce(testing::Return(true));

	fs.mount("/"s, std::move(mountRootOverride), Filesystem::PredecessorHidingPolicy::HIDE);

	auto mountDir = std::make_unique<MockMount>();
	EXPECT_CALL(*mountDir, list(Path("")))
		.WillRepeatedly(testing::Return(std::vector<std::string>{ "f"s }));
	EXPECT_CALL(*mountDir, exists(Path(""))).WillRepeatedly(testing::Return(true));
	EXPECT_CALL(*mountDir, openContents(Path("f"))).WillOnce(testing::Return("dir\0"s));
	EXPECT_CALL(*mountDir, exists(Path("f"))).WillOnce(testing::Return(true));

	fs.mount("/dir"s, std::move(mountDir), Filesystem::PredecessorHidingPolicy::HIDE);

	BOOST_CHECK(fs.list("/"s) == (std::vector<std::string>{ "f"s, "f2"s }));
	BOOST_CHECK(fs.list("/dir"s) == std::vector<std::string>{ "f"s });

	BOOST_CHECK_EQUAL(
		std::string(std::istreambuf_iterator<char>(*fs.open("/f"s)), {}), "root override\0"s);
	BOOST_CHECK_EQUAL(
		std::string(std::istreambuf_iterator<char>(*fs.open("/dir/f"s)), {}), "dir\0"s);
}

BOOST_AUTO_TEST_CASE(FilesAddedWhenAnotherMountAdds) {
	/*
	 * - Mounted at /:
	 * - f
	 * - f1
	 * - Mounted at /:
	 * - f
	 * - f2
	 */

	auto mountRoot = std::make_unique<MockMount>();
	EXPECT_CALL(*mountRoot, list(Path("")))
		.WillRepeatedly(testing::Return(std::vector<std::string>{ "f"s, "f1"s }));
	EXPECT_CALL(*mountRoot, exists(Path(""))).WillRepeatedly(testing::Return(true));
	EXPECT_CALL(*mountRoot, openContents(Path("f1"))).WillOnce(testing::Return("root\0"s));
	EXPECT_CALL(*mountRoot, exists(Path("f1"))).WillOnce(testing::Return(true));

	Filesystem fs;
	fs.mount("/"s, std::move(mountRoot), Filesystem::PredecessorHidingPolicy::HIDE);

	auto mountRootOverride = std::make_unique<MockMount>();
	EXPECT_CALL(*mountRootOverride, list(Path("")))
		.WillRepeatedly(testing::Return(std::vector<std::string>{ "f"s, "f2"s }));
	EXPECT_CALL(*mountRootOverride, exists(Path(""))).WillRepeatedly(testing::Return(true));
	EXPECT_CALL(*mountRootOverride, openContents(Path("f"))).WillOnce(testing::Return("root override\0"s));
	EXPECT_CALL(*mountRootOverride, exists(Path("f"))).WillOnce(testing::Return(true));
	EXPECT_CALL(*mountRootOverride, exists(Path("f1"))).WillOnce(testing::Return(false));
	EXPECT_CALL(*mountRootOverride, openContents(Path("f2"))).WillOnce(testing::Return("root override\0"s));
	EXPECT_CALL(*mountRootOverride, exists(Path("f2"))).WillOnce(testing::Return(true));

	fs.mount("/"s, std::move(mountRootOverride), Filesystem::PredecessorHidingPolicy::ADD);

	auto a = fs.list("/"s);
	BOOST_CHECK(a == (std::vector<std::string>{ "f"s, "f1"s, "f2"s }));

	BOOST_CHECK_EQUAL(
		std::string(std::istreambuf_iterator<char>(*fs.open("/f"s)), {}), "root override\0"s);
	BOOST_CHECK_EQUAL(
		std::string(std::istreambuf_iterator<char>(*fs.open("/f1"s)), {}), "root\0"s);
	BOOST_CHECK_EQUAL(
		std::string(std::istreambuf_iterator<char>(*fs.open("/f2"s)), {}), "root override\0"s);
}

BOOST_AUTO_TEST_CASE(ThrowsIfNoMountInPath) {
	using namespace std::string_literals;

	auto mount = std::make_unique<MockMount>();

	EXPECT_CALL(*mount, exists(Path("f"))).WillOnce(testing::Return(false));
	EXPECT_CALL(*mount, exists(Path("subdir"))).WillOnce(testing::Return(false));

	Filesystem fs;
	fs.mount("/dir"s, std::move(mount), Filesystem::PredecessorHidingPolicy::HIDE);

	BOOST_CHECK_THROW(fs.list("/"s), InvalidPath);
	BOOST_CHECK_THROW(fs.list("/no-dir"s), InvalidPath);
	BOOST_CHECK_THROW(fs.list("/dir/subdir"s), InvalidPath);
	BOOST_CHECK_THROW(fs.open("/"s), InvalidPath);
	BOOST_CHECK_THROW(fs.open("/no-dir"s), InvalidPath);
	BOOST_CHECK_THROW(fs.open("/dir/f"s), InvalidPath);
}

BOOST_AUTO_TEST_SUITE_END(/* MilkFsFilesystemTestSuite */);

} // anonymous namespace
