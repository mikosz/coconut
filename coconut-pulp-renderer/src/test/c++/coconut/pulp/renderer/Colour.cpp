#include <boost/test/auto_unit_test.hpp>

#include "coconut/pulp/renderer/Colour.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;

namespace /* anonymous */ {

BOOST_AUTO_TEST_SUITE(PulpRendererColourTestSuite);

BOOST_AUTO_TEST_CASE(DataReturnsRawDataPointer) {
	const auto rgbaColour = RGBA(0.1f, 0.2f, 0.3f, 0.4f);
	const auto* data = rgbaColour.data();

	BOOST_CHECK_EQUAL(data[0], 0.1f);
	BOOST_CHECK_EQUAL(data[1], 0.2f);
	BOOST_CHECK_EQUAL(data[2], 0.3f);
	BOOST_CHECK_EQUAL(data[3], 0.4f);
}

BOOST_AUTO_TEST_CASE(AccessesChannelsInRGBAColour) {
	const auto rgbaColour = RGBA(1.0f, 2.0f, 3.0f, 4.0f);
	BOOST_CHECK_EQUAL(rgbaColour.colourFormat(), ColourFormat::RGBA);
	BOOST_CHECK_EQUAL(rgbaColour.red(), 1.0f);
	BOOST_CHECK_EQUAL(rgbaColour.green(), 2.0f);
	BOOST_CHECK_EQUAL(rgbaColour.blue(), 3.0f);
	BOOST_CHECK_EQUAL(rgbaColour.alpha(), 4.0f);
}

BOOST_AUTO_TEST_CASE(AccessesChannelsInRGBColour) {
	const auto rgbColour = RGB(1.0f, 2.0f, 3.0f);
	BOOST_CHECK_EQUAL(rgbColour.colourFormat(), ColourFormat::RGB);
	BOOST_CHECK_EQUAL(rgbColour.red(), 1.0f);
	BOOST_CHECK_EQUAL(rgbColour.green(), 2.0f);
	BOOST_CHECK_EQUAL(rgbColour.blue(), 3.0f);
	BOOST_CHECK_EQUAL(rgbColour.alpha(), 0.0f);
}

BOOST_AUTO_TEST_CASE(AccessesChannelsInRedColour) {
	const auto redColour = Red(1.0f);
	BOOST_CHECK_EQUAL(redColour.colourFormat(), ColourFormat::R);
	BOOST_CHECK_EQUAL(redColour.red(), 1.0f);
	BOOST_CHECK_EQUAL(redColour.green(), 0.0f);
	BOOST_CHECK_EQUAL(redColour.blue(), 0.0f);
	BOOST_CHECK_EQUAL(redColour.alpha(), 0.0f);
}

BOOST_AUTO_TEST_CASE(AccessesChannelsInGreenColour) {
	const auto redColour = Green(1.0f);
	BOOST_CHECK_EQUAL(redColour.colourFormat(), ColourFormat::G);
	BOOST_CHECK_EQUAL(redColour.red(), 0.0f);
	BOOST_CHECK_EQUAL(redColour.green(), 1.0f);
	BOOST_CHECK_EQUAL(redColour.blue(), 0.0f);
	BOOST_CHECK_EQUAL(redColour.alpha(), 0.0f);
}

BOOST_AUTO_TEST_CASE(AccessesChannelsInBlueColour) {
	const auto redColour = Blue(1.0f);
	BOOST_CHECK_EQUAL(redColour.colourFormat(), ColourFormat::B);
	BOOST_CHECK_EQUAL(redColour.red(), 0.0f);
	BOOST_CHECK_EQUAL(redColour.green(), 0.0f);
	BOOST_CHECK_EQUAL(redColour.blue(), 1.0f);
	BOOST_CHECK_EQUAL(redColour.alpha(), 0.0f);
}

BOOST_AUTO_TEST_CASE(AccessesChannelsInAlphaColour) {
	const auto redColour = Alpha(1.0f);
	BOOST_CHECK_EQUAL(redColour.colourFormat(), ColourFormat::A);
	BOOST_CHECK_EQUAL(redColour.red(), 0.0f);
	BOOST_CHECK_EQUAL(redColour.green(), 0.0f);
	BOOST_CHECK_EQUAL(redColour.blue(), 0.0f);
	BOOST_CHECK_EQUAL(redColour.alpha(), 1.0f);
}

BOOST_AUTO_TEST_CASE(LiteralOperatorCreatesSingleChannelColours) {
	const auto halfRed = 0.5_r;
	BOOST_CHECK_EQUAL(halfRed.colourFormat(), ColourFormat::R);
	BOOST_CHECK_EQUAL(halfRed.red(), 0.5f);

	const auto quarterGreen = 0.25_g;
	BOOST_CHECK_EQUAL(quarterGreen.colourFormat(), ColourFormat::G);
	BOOST_CHECK_EQUAL(quarterGreen.green(), 0.25f);

	const auto oneEightBlue = 0.125_b;
	BOOST_CHECK_EQUAL(oneEightBlue.colourFormat(), ColourFormat::B);
	BOOST_CHECK_EQUAL(oneEightBlue.blue(), 0.125f);

	const auto oneAlpha = 1.0_a;
	BOOST_CHECK_EQUAL(oneAlpha.colourFormat(), ColourFormat::A);
	BOOST_CHECK_EQUAL(oneAlpha.alpha(), 1.0f);
}

BOOST_AUTO_TEST_SUITE_END(/* PulpRendererColourTestSuite */);

} // anonymous namespace
