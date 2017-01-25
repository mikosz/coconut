#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include <sstream>

#include <coconut-tools/test-utils/test-utils.hpp>
#include <coconut-tools/test-utils/GMockFixture.hpp>

#include "coconut/milk/fs/Cache.hpp"
#include "coconut/milk/fs/FilesystemContext.hpp"
#include "coconut/milk/fs/Filesystem.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::fs;

using namespace std::string_literals;

#if 0
// TODO: re-enable tests
namespace /* anonymous */ {

class MockMount : public Mount {
public:

	MOCK_CONST_METHOD1(list, std::vector<std::string> (const Path&));

	MOCK_CONST_METHOD1(openContents, std::string (const Path&));

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

	bool exists(const Path&) const override {
		return true;
	}

};

BOOST_FIXTURE_TEST_SUITE(MilkFsFilesystemContextTestSuite, coconut_tools::test_utils::GMockFixture);

BOOST_AUTO_TEST_CASE(ListsFilesInDirectory) {
	auto mountRootABC = std::make_unique<MockMount>();
	EXPECT_CALL(*mountRootABC, list(Path("")))
		.WillRepeatedly(testing::Return(std::vector<std::string>{ "f1"s, "f2"s, "f3"s }));

	auto mountRootAB = std::make_unique<MockMount>();
	EXPECT_CALL(*mountRootAB, list(Path("")))
		.WillRepeatedly(testing::Return(std::vector<std::string>{ "c"s }));

	auto fs = std::make_shared<Filesystem>();
	fs->mount("/a/b"s, std::move(mountRootAB), Filesystem::PredecessorHidingPolicy::HIDE);

	auto cache = std::make_shared<Cache>();

	auto fsContext = FilesystemContext(fs, cache);

	fsContext.mount("a/b/c"s, std::move(mountRootABC), Filesystem::PredecessorHidingPolicy::HIDE);

	BOOST_CHECK(fsContext.list("/a/b"s) == std::vector<std::string>{ "c"s });
	BOOST_CHECK(fsContext.list("/a/b/c"s) == (std::vector<std::string>{ "f1"s, "f2"s, "f3"s }));

	BOOST_CHECK(fsContext.list("a/b"s) == std::vector<std::string>{ "c"s });
	BOOST_CHECK(fsContext.list("a/b/c"s) == (std::vector<std::string>{ "f1"s, "f2"s, "f3"s }));

	fsContext.changeWorkingDirectory("/a");

	BOOST_CHECK(fsContext.list("b"s) == std::vector<std::string>{ "c"s });
	BOOST_CHECK(fsContext.list("b/c"s) == (std::vector<std::string>{ "f1"s, "f2"s, "f3"s }));

	fsContext.changeWorkingDirectory("b");

	BOOST_CHECK(fsContext.list("."s) == std::vector<std::string>{ "c"s });
	BOOST_CHECK(fsContext.list("c"s) == (std::vector<std::string>{ "f1"s, "f2"s, "f3"s }));

	BOOST_CHECK(fsContext.list("/a/b"s) == std::vector<std::string>{ "c"s });
	BOOST_CHECK(fsContext.list("/a/b/c"s) == (std::vector<std::string>{ "f1"s, "f2"s, "f3"s }));
}

BOOST_AUTO_TEST_CASE(OpensFiles) {
	auto mountRootABC = std::make_unique<MockMount>();
	EXPECT_CALL(*mountRootABC, openContents(Path("f")))
		.WillOnce(testing::Return("/a/b/c/f data\0"s));

	auto mountRootAB = std::make_unique<MockMount>();
	EXPECT_CALL(*mountRootAB, openContents(Path("f")))
		.WillOnce(testing::Return("/a/b/f data\0"s));

	auto fs = std::make_shared<Filesystem>();
	fs->mount("/a/b"s, std::move(mountRootAB), Filesystem::PredecessorHidingPolicy::HIDE);

	auto cache = std::make_shared<Cache>();

	auto fsContext = FilesystemContext(fs, cache);

	fsContext.mount("a/b/c"s, std::move(mountRootABC), Filesystem::PredecessorHidingPolicy::HIDE);

	BOOST_CHECK_EQUAL(reinterpret_cast<const char*>(fsContext.hint("/a/b/f"s).get()->data()), "/a/b/f data"s);
	BOOST_CHECK_EQUAL(reinterpret_cast<const char*>(fsContext.hint("/a/b/c/f"s).get()->data()), "/a/b/c/f data"s);

	BOOST_CHECK_EQUAL(reinterpret_cast<const char*>(fsContext.hint("a/b/f"s).get()->data()), "/a/b/f data"s);
	BOOST_CHECK_EQUAL(reinterpret_cast<const char*>(fsContext.hint("a/b/c/f"s).get()->data()), "/a/b/c/f data"s);

	fsContext.changeWorkingDirectory("/a");

	BOOST_CHECK_EQUAL(reinterpret_cast<const char*>(fsContext.hint("b/f"s).get()->data()), "/a/b/f data"s);
	BOOST_CHECK_EQUAL(reinterpret_cast<const char*>(fsContext.hint("b/c/f"s).get()->data()), "/a/b/c/f data"s);

	fsContext.changeWorkingDirectory("b");

	BOOST_CHECK_EQUAL(reinterpret_cast<const char*>(fsContext.hint("f"s).get()->data()), "/a/b/f data"s);
	BOOST_CHECK_EQUAL(reinterpret_cast<const char*>(fsContext.hint("c/f"s).get()->data()), "/a/b/c/f data"s);

	BOOST_CHECK_EQUAL(reinterpret_cast<const char*>(fsContext.hint("/a/b/f"s).get()->data()), "/a/b/f data"s);
	BOOST_CHECK_EQUAL(reinterpret_cast<const char*>(fsContext.hint("/a/b/c/f"s).get()->data()), "/a/b/c/f data"s);
}

BOOST_AUTO_TEST_SUITE_END(/* MilkFsFilesystemContextTestSuite */);

} // anonymous namespace

#endif
