#include <boost/test/auto_unit_test.hpp>

#include "coconut/milk/utils/sliceIdentifier.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::utils;

namespace /* anonymous */ {

BOOST_AUTO_TEST_SUITE(MilkUtilsIntegralValueTestSuite);

BOOST_AUTO_TEST_CASE(ReturnsEmptyForEmptyString) {
	BOOST_CHECK(sliceIdentifier<std::vector>("").empty());
}

BOOST_AUTO_TEST_CASE(ReturnsUnderscoreForSingleUnderscore) {
	const auto expected = std::vector<std::string>{ "_" };
	BOOST_CHECK(sliceIdentifier<std::vector>("_") == expected);
}

BOOST_AUTO_TEST_CASE(ReturnsAllLowercase) {
	const auto expected = std::vector<std::string>{ "i", "am", "mixed", "case" };
	BOOST_CHECK(sliceIdentifier<std::vector>("i_Am_Mixed_Case") == expected);
}

BOOST_AUTO_TEST_CASE(SlicesCamelCase) {
	const auto expected = std::vector<std::string>{ "i", "am", "camel", "case" };
	BOOST_CHECK(sliceIdentifier<std::vector>("iAmCamelCase") == expected);	
}

BOOST_AUTO_TEST_CASE(SlicesUnderscores) {
	const auto expected = std::vector<std::string>{ "i", "am", "underscored" };
	BOOST_CHECK(sliceIdentifier<std::vector>("i_am_underscored") == expected);	
}

BOOST_AUTO_TEST_CASE(SlicesMixed) {
	const auto expected = std::vector<std::string>{ "i", "am", "mixed" };
	BOOST_CHECK(sliceIdentifier<std::vector>("i_AmMixed") == expected);
}

BOOST_AUTO_TEST_SUITE_END(/* MilkUtilsIntegralValueTestSuite */);

} // anonymous namespace
