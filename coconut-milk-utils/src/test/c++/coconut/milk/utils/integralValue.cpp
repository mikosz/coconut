#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include "coconut/milk/utils/integralValue.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::utils;

namespace /* anonymous */ {

enum BasicEnum {
	ONE = 1,
};

enum class EnumClass {
	TWO = 2,
};

BOOST_AUTO_TEST_SUITE(MilkUtilsIntegralValueTestSuite);

BOOST_AUTO_TEST_CASE(ReturnsBasicEnumValue) {
	BOOST_CHECK_EQUAL(integralValue(ONE), 1);
}

BOOST_AUTO_TEST_CASE(ReturnsEnumClassValue) {
	BOOST_CHECK_EQUAL(integralValue(EnumClass::TWO), 2);
}

BOOST_AUTO_TEST_SUITE_END(/* MilkUtilsIntegralValueTestSuite */);

} // anonymous namespace
