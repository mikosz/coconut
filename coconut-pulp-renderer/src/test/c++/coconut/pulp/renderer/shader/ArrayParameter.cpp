#include <boost/test/auto_unit_test.hpp>

#include <coconut-tools/test-utils/GMockFixture.hpp>

#include "coconut/milk/utils/Empty.hpp"

#include "coconut/pulp/renderer/shader/ArrayParameter.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;
using namespace coconut::pulp::renderer::shader;

namespace /* anonymous */ {

template <size_t SIZE>
class MockParameter : public Parameter<milk::utils::Empty, size_t> {
public:

	MockParameter() :
		Parameter(SIZE)
	{
	}

	MOCK_METHOD3(update, void(void*, const milk::utils::Empty&, const size_t&));

};

BOOST_FIXTURE_TEST_SUITE(PulpRendererArrayParameterTestSuite, coconut_tools::test_utils::GMockFixture);

BOOST_AUTO_TEST_CASE(Requires16ByteAlignment) {
	ArrayParameter<milk::utils::Empty> parameter(std::make_unique<MockParameter<16>>(), 10);
	BOOST_CHECK(parameter.requires16ByteAlignment());
}

BOOST_AUTO_TEST_CASE(CalculatesCorrectSizeFor16ByteMultipleElements) {
	ArrayParameter<milk::utils::Empty> parameter(std::make_unique<MockParameter<2 * 16>>(), 10);
	BOOST_CHECK_EQUAL(parameter.size(), 2 * 16 * 10);
}

BOOST_AUTO_TEST_CASE(CalculatesCorrectSizeForElementsRequiringPadding) {
	ArrayParameter<milk::utils::Empty> parameter(std::make_unique<MockParameter<16 + 13>>(), 10);
	BOOST_CHECK_EQUAL(parameter.size(), 2 * 16 * 10);
}

BOOST_AUTO_TEST_CASE(DoesntAlign16ByteMultipleElements) {
	auto element = std::make_shared<MockParameter<16>>();
	ArrayParameter<milk::utils::Empty> parameter(element, 10);

	using testing::_;

	size_t expectedIndex = 0;

	EXPECT_CALL(*element, update(_, _, _))
		.Times(10)
		.WillRepeatedly(testing::Invoke([&expectedIndex](void* buffer, const milk::utils::Empty&, const size_t& index) {
			BOOST_REQUIRE_EQUAL(index, expectedIndex);
		
			auto* wordPtr = reinterpret_cast<std::uint32_t*>(buffer);
			wordPtr[0] = static_cast<std::uint32_t>(~0);
			wordPtr[1] = static_cast<std::uint32_t>(~0);
			wordPtr[2] = static_cast<std::uint32_t>(~0);
			wordPtr[3] = static_cast<std::uint32_t>(~0);

			++expectedIndex;
		}));

	std::vector<std::uint8_t> buffer(parameter.size(), 0);

	parameter.update(buffer.data(), milk::utils::Empty());

	for (size_t i = 0; i < parameter.size(); ++i) {
		BOOST_CHECK_EQUAL(buffer[i], static_cast<std::uint8_t>(~0));
	}
}

BOOST_AUTO_TEST_CASE(Aligns12ByteMultipleElementsTo16ByteBoundary) {
	auto element = std::make_shared<MockParameter<12>>();
	ArrayParameter<milk::utils::Empty> parameter(element, 10);

	using testing::_;

	size_t expectedIndex = 0;

	EXPECT_CALL(*element, update(_, _, _))
		.Times(10)
		.WillRepeatedly(testing::Invoke([&expectedIndex](void* buffer, const milk::utils::Empty&, const size_t& index) {
		BOOST_REQUIRE_EQUAL(index, expectedIndex);

		auto* wordPtr = reinterpret_cast<std::uint32_t*>(buffer);
		wordPtr[0] = static_cast<std::uint32_t>(~0);
		wordPtr[1] = static_cast<std::uint32_t>(~0);
		wordPtr[2] = static_cast<std::uint32_t>(~0);

		++expectedIndex;
	}));

	std::vector<std::uint8_t> buffer(parameter.size(), 0);

	parameter.update(buffer.data(), milk::utils::Empty());

	for (size_t i = 0; i < parameter.size(); ++i) {
		if (i % 16 < 12) {
			BOOST_CHECK_EQUAL(buffer[i], static_cast<std::uint8_t>(~0));
		} else {
			BOOST_CHECK_EQUAL(buffer[i], 0);
		}
	}
}

BOOST_AUTO_TEST_SUITE_END(/* PulpRendererArrayParameterTestSuite */);

} // anonymous namespace
