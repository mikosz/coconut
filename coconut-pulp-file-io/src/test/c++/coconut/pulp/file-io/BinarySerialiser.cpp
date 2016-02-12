#include <boost/test/auto_unit_test.hpp>

#include <algorithm>
#include <sstream>

#include "coconut/pulp/file-io/BinarySerialiser.hpp"

#include "coconut/milk/utils/endianness.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::file_io;

namespace /* anonymous */ {

struct BasicTypeStruct {
};

struct SubStruct {
};

void serialise(Serialiser& s, const SubStruct&) {
	s << Serialiser::Label("int") << 42;
}

void serialise(Serialiser& s, const BasicTypeStruct&) {
	s << Serialiser::Label("int") << 123;
	s << Serialiser::Label("float") << 1.3f;
	s << Serialiser::Label("string") << "value";

	std::vector<SubStruct> v(2);
	s << Serialiser::Label("sub-objects") << v;
}

BOOST_AUTO_TEST_SUITE(PulpFileIOBinarySerialiserTestSuite);

BOOST_AUTO_TEST_CASE(ThrowsIfFileExists) {
	BOOST_FAIL("implement me");
}

BOOST_AUTO_TEST_CASE(ThrowsIfFailedWrite) {
	BOOST_FAIL("implement me");
}

BOOST_AUTO_TEST_CASE(SerialisesDataToBinary) {
	std::ostringstream oss;
	BinarySerialiser s(oss);

	s << BasicTypeStruct();

	union {
		float f;
		char bytes[4];
	} onePointThree;

	onePointThree.f = 1.3f;

	const char expectedBigEndian[] = {
		1,
		0, 0, 0, 123,
		onePointThree.bytes[0], onePointThree.bytes[1], onePointThree.bytes[2], onePointThree.bytes[3],
		'v', 'a', 'l', 'u', 'e', '\0',
		0, 0, 0, 2,
		0, 0, 0, 42,
		0, 0, 0, 42,
		'\0'
	};

	const char expectedLittleEndian[] = {
		0,
		123, 0, 0, 0,
		onePointThree.bytes[0], onePointThree.bytes[1], onePointThree.bytes[2], onePointThree.bytes[3],
		'v', 'a', 'l', 'u', 'e', '\0',
		2, 0, 0, 0,
		42, 0, 0, 0,
		42, 0, 0, 0,
		'\0'
	};

	const auto&& serialisedData = oss.str();
	BOOST_CHECK_EQUAL(sizeof(expectedBigEndian) - 1, serialisedData.length());

	if (milk::utils::isBigEndian()) {
		const auto mismatch = std::mismatch(serialisedData.begin(), serialisedData.end(), expectedBigEndian);
		BOOST_CHECK(mismatch.first == serialisedData.end());
	} else {
		const auto mismatch = std::mismatch(serialisedData.begin(), serialisedData.end(), expectedLittleEndian);
		BOOST_CHECK(mismatch.first == serialisedData.end());
	}
}

BOOST_AUTO_TEST_SUITE_END(/* PulpFileIOBinarySerialiserTestSuite */);

} // anonymous namespace
