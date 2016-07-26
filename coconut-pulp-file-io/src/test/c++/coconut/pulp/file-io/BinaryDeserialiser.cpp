#include <boost/test/auto_unit_test.hpp>

#include <algorithm>
#include <sstream>
#include <fstream>

#include <coconut-tools/test-utils/test-utils.hpp>

#include "coconut/pulp/file-io/BinaryDeserialiser.hpp"

#include "coconut/milk/utils/endianness.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::file_io;

namespace /* anonymous */ {

std::istream* testIs = nullptr;

struct SubStruct {
	int i;
};

struct BasicTypeStruct {
	int i;
	float f;
	std::string s;
	bool b;
	std::vector<SubStruct> ss;
};

void serialise(Deserialiser& s, SubStruct& ss) {
	s >> Deserialiser::Label("int") >> ss.i;
}

void serialise(Deserialiser& s, BasicTypeStruct& bts) {
	s >> Deserialiser::Label("int") >> bts.i;
	s >> Deserialiser::Label("float") >> bts.f;
	s >> Deserialiser::Label("string") >> bts.s;
	s >> Deserialiser::Label("bool") >> bts.b;

	std::vector<SubStruct> v(2);
	s >> Deserialiser::Label("sub-objects") >> bts.ss;
}

struct BadStruct {
};

void serialise(Deserialiser& s, const BadStruct&) {
	assert(testIs != nullptr);
	testIs->setstate(std::ios::badbit);
}

struct FailStruct {
};

void serialise(Deserialiser& s, const FailStruct&) {
	assert(testIs != nullptr);
	testIs->setstate(std::ios::failbit);
}

BOOST_AUTO_TEST_SUITE(PulpFileIOBinaryDeserialiserTestSuite);

BOOST_FIXTURE_TEST_CASE(ThrowsIfBadPath, coconut_tools::test_utils::ResourcesDirFixture) {
	std::ifstream ifs((resourcesDir() / "no-such-dir" / "file.json").c_str());

	BOOST_CHECK_THROW(BinaryDeserialiser s(ifs), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(ThrowsIfFailedRead) {
	const char data[] = {
		1,
	};
	const auto dataSize = sizeof(data) / sizeof(data[0]);

	std::istringstream iss(std::string(data, data + dataSize));
	testIs = &iss;

	BinaryDeserialiser s(iss);

	BOOST_CHECK_THROW(s >> BadStruct(), std::runtime_error);

	iss.clear();

	BOOST_CHECK_THROW(s >> FailStruct(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(DeserialisesDataFromBinaryBigEndian) {
	union {
		float f;
		char bytes[4];
	} onePointThree;

	onePointThree.f = 1.3f;

	const char data[] = {
		1,
		0, 0, 0, 123,
		onePointThree.bytes[0], onePointThree.bytes[1], onePointThree.bytes[2], onePointThree.bytes[3],
		'v', 'a', 'l', 'u', 'e', '\0',
		1,
		0, 0, 0, 2,
		0, 0, 0, 42,
		0, 0, 0, 42,
		'\0'
	};
	const auto dataSize = sizeof(data) / sizeof(data[0]);

	std::istringstream iss(std::string(data, data + dataSize));
	BinaryDeserialiser s(iss);

	BasicTypeStruct bts;
	s >> bts;

	BOOST_CHECK_EQUAL(bts.i, 123);
	BOOST_CHECK_EQUAL(bts.f, 1.3f);
	BOOST_CHECK_EQUAL(bts.s, "value");
	BOOST_CHECK_EQUAL(bts.b, true);
	BOOST_ASSERT(bts.ss.size() == 2);
	BOOST_CHECK_EQUAL(bts.ss[0].i, 42);
	BOOST_CHECK_EQUAL(bts.ss[1].i, 42);
}

BOOST_AUTO_TEST_CASE(DeserialisesDataFromBinaryLittleEndian) {
	union {
		float f;
		char bytes[4];
	} onePointThree;

	onePointThree.f = 1.3f;

	const char data[] = {
		0,
		123, 0, 0, 0,
		onePointThree.bytes[0], onePointThree.bytes[1], onePointThree.bytes[2], onePointThree.bytes[3],
		'v', 'a', 'l', 'u', 'e', '\0',
		1,
		2, 0, 0, 0,
		42, 0, 0, 0,
		42, 0, 0, 0,
		'\0'
	};
	const auto dataSize = sizeof(data) / sizeof(data[0]);

	std::istringstream iss(std::string(data, data + dataSize));
	BinaryDeserialiser s(iss);

	BasicTypeStruct bts;
	s >> bts;

	BOOST_CHECK_EQUAL(bts.i, 123);
	BOOST_CHECK_EQUAL(bts.f, 1.3f);
	BOOST_CHECK_EQUAL(bts.s, "value");
	BOOST_CHECK_EQUAL(bts.b, true);
	BOOST_ASSERT(bts.ss.size() == 2);
	BOOST_CHECK_EQUAL(bts.ss[0].i, 42);
	BOOST_CHECK_EQUAL(bts.ss[1].i, 42);
}

BOOST_AUTO_TEST_SUITE_END(/* PulpFileIOBinaryDeserialiserTestSuite */);

} // anonymous namespace
