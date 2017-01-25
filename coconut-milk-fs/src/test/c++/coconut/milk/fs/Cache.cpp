#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include <coconut-tools/exceptions/LogicError.hpp>

#include "coconut/milk/fs/Cache.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::fs;

using namespace std::string_literals;

namespace /* anonymous */ {

BOOST_AUTO_TEST_SUITE(MilkFsCacheTestSuite);

BOOST_AUTO_TEST_CASE(HasReturnsWhetherCacheContainsAFile) {
	auto cache = Cache();

	BOOST_CHECK(!cache.has("/doesnt-exist"));
	BOOST_CHECK(!cache.has("/exists"));

	cache.store("/exists", std::async([]() { return SharedRawData(); }));

	BOOST_CHECK(!cache.has("/doesnt-exist"));
	BOOST_CHECK(cache.has("/exists"));
}

BOOST_AUTO_TEST_CASE(StoreKeepsAndGetReturnsProvidedDataFuture) {
	auto cache = Cache();

	BOOST_CHECK_THROW(cache.get("/file"), coconut_tools::exceptions::LogicError);

	auto data = std::make_shared<RawData>();
	auto future = cache.store("/file", std::async([&data]() -> SharedRawData { return data; }));

	BOOST_CHECK_EQUAL(future.get(), data);
	BOOST_CHECK_EQUAL(cache.get("/file").get(), data);
}

BOOST_AUTO_TEST_CASE(StoreThrowsOnDuplicatedFile) {
	auto cache = Cache();

	cache.store("/file", std::async([]() { return SharedRawData(); }));
	
	BOOST_CHECK_THROW(
		cache.store("/file", std::async([]() { return SharedRawData(); })),
		coconut_tools::exceptions::LogicError
		);
}

BOOST_AUTO_TEST_CASE(InvalidateRemovesStoredFile) {
	auto cache = Cache();

	cache.store("/file", std::async([]() { return SharedRawData(); }));
	cache.invalidate("/file");
	cache.invalidate("/no-op");

	BOOST_CHECK(!cache.has("/file"));

	cache.store("/file", std::async([]() { return SharedRawData(); }));

	BOOST_CHECK(cache.has("/file"));
}

BOOST_AUTO_TEST_CASE(InvalidateChildrenRemovesAllFilesInDirectory) {
	auto cache = Cache();

	cache.store("/file", std::async([]() { return SharedRawData(); }));
	cache.store("/directory/file", std::async([]() { return SharedRawData(); }));
	cache.store("/directory/subdirectory/file", std::async([]() { return SharedRawData(); }));
	cache.store("/other-directory/file", std::async([]() { return SharedRawData(); }));

	cache.invalidateChildren("/directory");
	cache.invalidateChildren("/no-op");

	BOOST_CHECK(cache.has("/file"));
	BOOST_CHECK(!cache.has("/directory/file"));
	BOOST_CHECK(!cache.has("/directory/subdirectory/file"));
	BOOST_CHECK(cache.has("/other-directory/file"));
}

BOOST_AUTO_TEST_SUITE_END(/* MilkFsCacheTestSuite */);

} // anonymous namespace
