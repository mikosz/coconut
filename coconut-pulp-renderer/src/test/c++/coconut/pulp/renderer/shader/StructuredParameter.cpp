#include <boost/test/auto_unit_test.hpp>

#include <coconut-tools/test-utils/GMockFixture.hpp>

#include "coconut/pulp/renderer/shader/StructuredParameter.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;
using namespace coconut::pulp::renderer::shader;

namespace /* anonymous */ {

template <size_t SIZE>
class MockParameter : public Parameter<> {
public:

	MockParameter() :
		Parameter(SIZE)
	{
	}

	void update(void* buffer) override {
		std::memset(buffer, 0xff, SIZE);
	}

	MOCK_CONST_METHOD0(requires16ByteAlignment, bool ());

};

BOOST_FIXTURE_TEST_SUITE(PulpRendererStructuredParameterTestSuite, coconut_tools::test_utils::GMockFixture);

BOOST_AUTO_TEST_CASE(Requires16ByteAlignment) {
	StructuredParameter<>::PaddedSubparameters subparameters;
	StructuredParameter<> parameter(subparameters);
	BOOST_CHECK(parameter.requires16ByteAlignment());
}

BOOST_AUTO_TEST_CASE(DoesntAddPaddingToAlignedParameters) {
	StructuredParameter<>::Subparameters subparameters;

	auto matrixParameter = std::make_shared<MockParameter<4 * 16>>();
	EXPECT_CALL(*matrixParameter, requires16ByteAlignment()).WillRepeatedly(testing::Return(false));
	subparameters.emplace_back(matrixParameter);

	auto oneWordParameter = std::make_shared<MockParameter<4>>();
	EXPECT_CALL(*oneWordParameter, requires16ByteAlignment()).WillRepeatedly(testing::Return(false));
	subparameters.emplace_back(oneWordParameter);

	auto threeWordParameter = std::make_shared<MockParameter<12>>();
	EXPECT_CALL(*threeWordParameter, requires16ByteAlignment()).WillRepeatedly(testing::Return(false));
	subparameters.emplace_back(threeWordParameter);

	auto paddedSubparameters = layoutSubparameters<>(subparameters);

	BOOST_REQUIRE_EQUAL(paddedSubparameters.size(), 3);
	BOOST_CHECK_EQUAL(paddedSubparameters[0].padding, 0);
	BOOST_CHECK_EQUAL(paddedSubparameters[1].padding, 0);
	BOOST_CHECK_EQUAL(paddedSubparameters[2].padding, 0);

	StructuredParameter<> parameter(paddedSubparameters);
	
	BOOST_REQUIRE_EQUAL(parameter.size(), 5 * 16);

	std::vector<std::uint8_t> buffer(parameter.size(), 0);

	parameter.update(buffer.data());

	std::vector<std::uint8_t> expected(parameter.size(), 0xff);

	BOOST_CHECK(buffer == expected);
}

BOOST_AUTO_TEST_CASE(AddsPaddingToParametersCrossing16ByteBorder) {
	StructuredParameter<>::Subparameters subparameters;

	auto sixWordParameter = std::make_shared<MockParameter<16 + 8>>();
	EXPECT_CALL(*sixWordParameter, requires16ByteAlignment()).WillRepeatedly(testing::Return(false));
	subparameters.emplace_back(sixWordParameter);

	auto fourWordParameter = std::make_shared<MockParameter<16>>();
	EXPECT_CALL(*fourWordParameter, requires16ByteAlignment()).WillRepeatedly(testing::Return(false));
	subparameters.emplace_back(fourWordParameter);

	auto paddedSubparameters = layoutSubparameters<>(subparameters);

	BOOST_REQUIRE_EQUAL(paddedSubparameters.size(), 2);
	BOOST_CHECK_EQUAL(paddedSubparameters[0].padding, 0);
	BOOST_CHECK_EQUAL(paddedSubparameters[1].padding, 8);

	StructuredParameter<> parameter(paddedSubparameters);

	BOOST_REQUIRE_EQUAL(parameter.size(), (16 + 8) + 8 + (16));

	std::vector<std::uint8_t> buffer(parameter.size(), 0);

	parameter.update(buffer.data());

	std::vector<std::uint8_t> expected(parameter.size(), 0xff);
	std::memset(expected.data() + sixWordParameter->size(), 0x00, 8);

	BOOST_CHECK(buffer == expected);
}

BOOST_AUTO_TEST_CASE(AddsPaddingToRequiredParametersWhenTheyNeedPadding) {
	StructuredParameter<>::Subparameters subparameters;

	auto sixWordParameter = std::make_shared<MockParameter<16 + 8>>();
	EXPECT_CALL(*sixWordParameter, requires16ByteAlignment()).WillRepeatedly(testing::Return(true));
	subparameters.emplace_back(sixWordParameter);

	auto misalignedOneWordParameter = std::make_shared<MockParameter<8>>();
	EXPECT_CALL(*misalignedOneWordParameter, requires16ByteAlignment()).WillRepeatedly(testing::Return(true));
	subparameters.emplace_back(misalignedOneWordParameter);

	auto paddingOneWordParameter = std::make_shared<MockParameter<8>>();
	EXPECT_CALL(*paddingOneWordParameter, requires16ByteAlignment()).WillRepeatedly(testing::Return(false));
	subparameters.emplace_back(paddingOneWordParameter);

	auto alignedFourWordParameter = std::make_shared<MockParameter<16>>();
	EXPECT_CALL(*alignedFourWordParameter, requires16ByteAlignment()).WillRepeatedly(testing::Return(true));
	subparameters.emplace_back(alignedFourWordParameter);

	auto paddedSubparameters = layoutSubparameters<>(subparameters);

	BOOST_REQUIRE_EQUAL(paddedSubparameters.size(), 4);
	BOOST_CHECK_EQUAL(paddedSubparameters[0].padding, 0);
	BOOST_CHECK_EQUAL(paddedSubparameters[1].padding, 8);
	BOOST_CHECK_EQUAL(paddedSubparameters[2].padding, 0);
	BOOST_CHECK_EQUAL(paddedSubparameters[3].padding, 0);

	StructuredParameter<> parameter(paddedSubparameters);

	BOOST_REQUIRE_EQUAL(parameter.size(), (16 + 8) + 8 + (8) + (8) + (16));

	std::vector<std::uint8_t> buffer(parameter.size(), 0);

	parameter.update(buffer.data());

	std::vector<std::uint8_t> expected(parameter.size(), 0xff);
	std::memset(expected.data() + sixWordParameter->size(), 0x00, 8);

	BOOST_CHECK(buffer == expected);
}

BOOST_AUTO_TEST_SUITE_END(/* PulpRendererStructuredParameterTestSuite */);

} // anonymous namespace
