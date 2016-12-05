#include <boost/test/auto_unit_test.hpp>

#include <sstream>

#include <coconut-tools/test-utils/test-utils.hpp>
#include <coconut-tools/test-utils/GMockFixture.hpp>

#include "coconut/milk/fs/Filesystem.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::fs;

namespace /* anonymous */ {

class MockMount : public Mount {
public:

	MOCK_CONST_METHOD1(list, std::vector<std::string> (const Path&));

	IStream open(const Path& path) const override {
		return IStream();
	}

};

BOOST_FIXTURE_TEST_SUITE(MilkFsFilesystemTestSuite, coconut_tools::test_utils::GMockFixture);

BOOST_AUTO_TEST_CASE(ListsFilesInDirectory) {
	using namespace std::string_literals;

	auto mountRootABC = std::make_unique<MockMount>();
	EXPECT_CALL(*mountRootABC, list(Path(""s)))
		.WillOnce(testing::Return(std::vector<std::string>{ "f1"s, "f2"s, "f3"s }));

	auto mountRootAB = std::make_unique<MockMount>();
	EXPECT_CALL(*mountRootAB, list(Path(""s)))
		.WillOnce(testing::Return(std::vector<std::string>{ "c"s }));

	Filesystem fs;
	fs.mount("a/b"s, std::move(mountRootAB));
	fs.mount("a/b/c"s, std::move(mountRootABC));

	BOOST_CHECK(fs.list("a/b"s) == std::vector<std::string>{ "c"s });
	BOOST_CHECK(fs.list("a/b/c"s) == (std::vector<std::string>{ "f1"s, "f2"s, "f3"s }));
}

BOOST_AUTO_TEST_SUITE_END(/* MilkFsFilesystemTestSuite */);

} // anonymous namespace
