#include <boost/test/auto_unit_test.hpp>

#include <sstream>
#include <fstream>
#include <chrono>
#include <atomic>

#include "coconut/milk/fs/Cache.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::fs;

using namespace std::string_literals;

namespace /* anonymous */ {

BOOST_AUTO_TEST_SUITE(MilkFsCacheTestSuite);

BOOST_AUTO_TEST_CASE(LoadReturnsDataFuture) {
	BOOST_FAIL("commented out");
/*	Cache cache;
	auto future = cache.load(
		"path",
		[]() -> IStream {
			return std::make_unique<std::istringstream>("data"s + '\0');
		}
		);

	BOOST_REQUIRE(future.valid());
	BOOST_CHECK_EQUAL(future.get()->size(), 5);
	BOOST_CHECK_EQUAL(reinterpret_cast<const char*>(future.get()->data()), "data");
}

BOOST_AUTO_TEST_CASE(DuplicateLoadWillNotSpawnAnotherProcess) {
	std::atomic<size_t> istreamOpenedTimes = 0u;

	const auto path = "path"s;
	const auto loader = [&istreamOpenedTimes]() -> IStream {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			++istreamOpenedTimes;
			return std::make_unique<std::istringstream>("data"s + '\0');
		};

	Cache cache;
	auto loadingFuture = cache.load(path, loader);
	auto whileLoadingFuture = cache.load(path, loader);

	BOOST_REQUIRE(loadingFuture.valid());
	BOOST_CHECK_EQUAL(loadingFuture.get()->size(), 5);
	BOOST_CHECK_EQUAL(reinterpret_cast<const char*>(loadingFuture.get()->data()), "data");

	BOOST_CHECK(whileLoadingFuture.valid());
	BOOST_CHECK_EQUAL(whileLoadingFuture.get()->size(), 5);
	BOOST_CHECK_EQUAL(whileLoadingFuture.get()->data(), loadingFuture.get()->data());

	BOOST_CHECK_EQUAL(istreamOpenedTimes, 1);

	auto postLoadingFuture = cache.load(path, loader);
	BOOST_REQUIRE(postLoadingFuture.valid());
	BOOST_CHECK_EQUAL(postLoadingFuture.get()->size(), 5);
	BOOST_CHECK_EQUAL(postLoadingFuture.get()->data(), loadingFuture.get()->data());

	BOOST_CHECK_EQUAL(istreamOpenedTimes, 1);
}

BOOST_AUTO_TEST_CASE(ThrowsExceptionOnReadErrors) {
	Cache cache;
	auto future = cache.load(
		"path",
		[]() -> IStream {
			auto iss = std::make_unique<std::istringstream>();
			iss->setstate(std::ios::badbit);
			return iss;
		}
		);

	BOOST_REQUIRE(future.valid());
	BOOST_CHECK_THROW(future.get(), std::runtime_error); */
}

BOOST_AUTO_TEST_SUITE_END(/* MilkFsCacheTestSuite */);

} // anonymous namespace
