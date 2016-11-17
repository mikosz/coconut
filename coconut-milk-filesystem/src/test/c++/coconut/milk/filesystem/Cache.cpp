#include <boost/test/auto_unit_test.hpp>

#include <sstream>

#include "coconut/milk/filesystem/Cache.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::filesystem;

namespace /* anonymous */ {

BOOST_AUTO_TEST_SUITE(MilkFilesystemCacheTestSuite);

BOOST_AUTO_TEST_CASE(LoadReturnsDataFuture) {
	auto istreamOpenedTimes = 0u;

	Cache cache;
	auto future = cache.load(
		"path",
		[&istreamOpenedTimes]() -> IStream {
			++istreamOpenedTimes;
			return std::make_unique<std::istringstream>("data");
		}
		);

	future.wait();
	BOOST_REQUIRE(future.valid());
	BOOST_CHECK_EQUAL(istreamOpenedTimes, 1);
	BOOST_CHECK_EQUAL(reinterpret_cast<const char*>(future.get().data()), "data");
}

BOOST_AUTO_TEST_CASE(DuplicateLoadWillNotSpawnAnotherProcess) {
	auto istreamOpenedTimes = 0u;

	Cache cache;
	auto future = cache.load(
		"path",
		[&istreamOpenedTimes]() -> IStream {
			++istreamOpenedTimes;
			return std::make_unique<std::istringstream>("data");
		}
		);

	future.wait();
	BOOST_REQUIRE(future.valid());
	BOOST_CHECK_EQUAL(istreamOpenedTimes, 1);
	BOOST_CHECK_EQUAL(reinterpret_cast<const char*>(future.get().data()), "data");
}

BOOST_AUTO_TEST_CASE(LoadOfALoadedPathReturnsCachedData) {
}

BOOST_AUTO_TEST_SUITE_END(/* MilkFilesystemCacheTestSuite */);

} // anonymous namespace
