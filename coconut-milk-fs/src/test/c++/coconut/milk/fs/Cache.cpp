#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include <sstream>
#include <fstream>
#include <chrono>
#include <atomic>

#include <coconut-tools/test-utils/GMockFixture.hpp>

#include "coconut/milk/fs/Cache.hpp"
#include "coconut/milk/fs/Filesystem.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::fs;

using namespace std::string_literals;

namespace /* anonymous */ {

class MockMount : public Mount {
public:

	MockMount(bool badOpen) :
		badOpen_(badOpen)
	{
	}

	MOCK_CONST_METHOD1(list, std::vector<std::string> (const Path&));

	MOCK_CONST_METHOD1(openContents, std::string (const Path&));
	
	bool exists(const Path&) const override {
		return true;
	}

	IStream open(const Path& path) const override {
		if (badOpen_) {
			auto iss = std::make_unique<std::istringstream>("");
			iss->setstate(std::ios::badbit);
			return std::move(iss);
		} else {
			return std::make_unique<std::istringstream>(openContents(path));
		}
	}

	bool badOpen_;

};

BOOST_AUTO_TEST_SUITE(MilkFsCacheTestSuite);

BOOST_AUTO_TEST_CASE(LoadReturnsDataFuture) {
	Filesystem fs;
	auto mount = std::make_unique<MockMount>(false);
	
	EXPECT_CALL(*mount, openContents(Path("f"))).WillOnce(testing::Return("data\0"s));

	fs.mount("/", std::move(mount), Filesystem::PredecessorHidingPolicy::HIDE);

	Cache cache;
	auto future = cache.load(fs, "/f");

	BOOST_REQUIRE(future.valid());
	BOOST_CHECK_EQUAL(future.get()->size(), 5);
	BOOST_CHECK_EQUAL(reinterpret_cast<const char*>(future.get()->data()), "data");
}

BOOST_AUTO_TEST_CASE(DuplicateLoadWillNotSpawnAnotherThread) {
	std::atomic<size_t> istreamOpenedTimes = 0u;

	const auto path = "/f"s;
	const auto loader = [&istreamOpenedTimes]() {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			++istreamOpenedTimes;
			return "data\0"s;
		};

	Filesystem fs;
	auto mount = std::make_unique<MockMount>(false);
	
	EXPECT_CALL(*mount, openContents(Path("f"))).WillOnce(testing::InvokeWithoutArgs(loader));

	fs.mount("/", std::move(mount), Filesystem::PredecessorHidingPolicy::HIDE);

	Cache cache;
	auto loadingFuture = cache.load(fs, path);
	auto whileLoadingFuture = cache.load(fs, path);

	BOOST_REQUIRE(loadingFuture.valid());
	BOOST_CHECK_EQUAL(loadingFuture.get()->size(), 5);
	BOOST_CHECK_EQUAL(reinterpret_cast<const char*>(loadingFuture.get()->data()), "data");

	BOOST_CHECK(whileLoadingFuture.valid());
	BOOST_CHECK_EQUAL(whileLoadingFuture.get()->size(), 5);
	BOOST_CHECK_EQUAL(whileLoadingFuture.get()->data(), loadingFuture.get()->data());

	BOOST_CHECK_EQUAL(istreamOpenedTimes, 1);

	auto postLoadingFuture = cache.load(fs, path);
	BOOST_REQUIRE(postLoadingFuture.valid());
	BOOST_CHECK_EQUAL(postLoadingFuture.get()->size(), 5);
	BOOST_CHECK_EQUAL(postLoadingFuture.get()->data(), loadingFuture.get()->data());

	BOOST_CHECK_EQUAL(istreamOpenedTimes, 1);
}

BOOST_AUTO_TEST_CASE(ThrowsExceptionOnReadErrors) {
	Cache cache;

	Filesystem fs;
	auto mount = std::make_unique<MockMount>(true);

	fs.mount("/", std::move(mount), Filesystem::PredecessorHidingPolicy::HIDE);

	auto future = cache.load(fs, "/f"s);

	BOOST_REQUIRE(future.valid());
	BOOST_CHECK_THROW(future.get(), FailedToReadData);
}

BOOST_AUTO_TEST_SUITE_END(/* MilkFsCacheTestSuite */);

} // anonymous namespace
