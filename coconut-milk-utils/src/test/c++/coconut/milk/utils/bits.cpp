#include <boost/test/auto_unit_test.hpp>

#include "coconut/milk/utils/bits.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::utils;

namespace /* anonymous */ {

BOOST_AUTO_TEST_SUITE(MilkUtilsBitsTestSuite);

BOOST_AUTO_TEST_CASE(NumberOfBitsOnReturnsCorrectValue) {
	BOOST_CHECK_EQUAL(numberOfBitsOn(0b0), 0);
	BOOST_CHECK_EQUAL(numberOfBitsOn(0b1), 1);
	BOOST_CHECK_EQUAL(numberOfBitsOn(0b10), 1);
	BOOST_CHECK_EQUAL(numberOfBitsOn(0b11), 2);
	BOOST_CHECK_EQUAL(numberOfBitsOn(0b10110011100011110000), 10);
}

BOOST_AUTO_TEST_SUITE_END(/* MilkUtilsBitsTestSuite */);

} // anonymous namespace
