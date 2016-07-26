#include <boost/test/auto_unit_test.hpp>

#include <coconut-tools/test-utils/GMockFixture.hpp>

#include "coconut/pulp/file-io/Serialiser.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::file_io;

namespace /* anonymous */ {

class MockSerialiser : public Serialiser {
public:

	MOCK_METHOD1(write, void(const std::string&));

protected:

	void writeObjectStart() override {
	}

	void writeObjectEnd() override {
	}

	void writeArrayStart(std::uint32_t elementCount) override {
	}

	void writeArrayEnd() override {
	}

	void writeLabel(const std::string& label) override {
	}

	void write(bool i) override {
	}

	void write(std::uint8_t i) override {
	}

	void write(std::int8_t i) override {
	}

	void write(std::uint16_t i) override {
	}

	void write(std::int16_t i) override {
	}

	void write(std::uint32_t i) override {
	}

	void write(std::int32_t i) override {
	}

	void write(std::uint64_t i) override {
	}

	void write(std::int64_t i) override {
	}

	void write(float f) override {
	}

};

enum class EnumType {
	ENUM_VALUE_0,
	ENUM_VALUE_1,
};

std::string toString(EnumType enumValue) {
	switch (enumValue) {
	case EnumType::ENUM_VALUE_0:
		return "ENUM_VALUE_0";
	case EnumType::ENUM_VALUE_1:
		return "ENUM_VALUE_1";
	default:
		BOOST_FAIL("Unexpected enum value: " + std::to_string(static_cast<std::underlying_type_t<EnumType>>(enumValue)));
		return "";
	}
}

BOOST_FIXTURE_TEST_SUITE(PulpFileIOSerialiserTestSuite, coconut_tools::test_utils::GMockFixture);

BOOST_AUTO_TEST_CASE(SerialisesEnumsAsString) {
	MockSerialiser s;

	testing::InSequence sequence;
	EXPECT_CALL(s, write("ENUM_VALUE_0"));
	EXPECT_CALL(s, write("ENUM_VALUE_1"));

	s << EnumType::ENUM_VALUE_0;
	s << EnumType::ENUM_VALUE_1;
}

BOOST_AUTO_TEST_SUITE_END(/* PulpFileIOSerialiserTestSuite */);

} // anonymous namespace
