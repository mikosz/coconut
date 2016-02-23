#include <boost/test/auto_unit_test.hpp>

#include <coconut-tools/test-utils/GMockFixture.hpp>

#include "coconut/pulp/file-io/Deserialiser.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::file_io;

namespace /* anonymous */ {

class MockDeserialiser : public Deserialiser {
public:

	MOCK_METHOD1(read, void(std::string&));

protected:

	void readObjectStart() override {
	}

	std::uint32_t readArrayStart() override {
		return 0;
	}

	void readLabel(const std::string& label) override {
	}

	void read(std::uint8_t& i) override {
	}

	void read(std::int8_t& i) override {
	}

	void read(std::uint16_t& i) override {
	}

	void read(std::int16_t& i) override {
	}

	void read(std::uint32_t& i) override {
	}

	void read(std::int32_t& i) override {
	}

	void read(std::uint64_t& i) override {
	}

	void read(std::int64_t& i) override {
	}

	void read(float& f) override {
	}

};

enum class EnumType {
	ENUM_VALUE_0,
	ENUM_VALUE_1,
};

void fromString(EnumType& enumValue, const std::string& s) {
	if (s == "ENUM_VALUE_0") {
		enumValue = EnumType::ENUM_VALUE_0;
	} else if (s == "ENUM_VALUE_1") {
		enumValue = EnumType::ENUM_VALUE_1;
	} else {
		BOOST_FAIL("Unexpected enum value name: " + s);
	}
}

BOOST_FIXTURE_TEST_SUITE(PulpFileIODeserialiserTestSuite, coconut_tools::test_utils::GMockFixture);

BOOST_AUTO_TEST_CASE(DeserialisesEnumsFromString) {
	MockDeserialiser s;

	using testing::_;

	EXPECT_CALL(s, read(_))
		.WillOnce(testing::SetArgReferee<0>("ENUM_VALUE_0"))
		.WillOnce(testing::SetArgReferee<0>("ENUM_VALUE_1"))
		;

	EnumType enumValue;
	s >> enumValue;

	BOOST_CHECK(enumValue == EnumType::ENUM_VALUE_0);

	s >> enumValue;

	BOOST_CHECK(enumValue == EnumType::ENUM_VALUE_1);
}

BOOST_AUTO_TEST_SUITE_END(/* PulpFileIODeserialiserTestSuite */);

} // anonymous namespace
