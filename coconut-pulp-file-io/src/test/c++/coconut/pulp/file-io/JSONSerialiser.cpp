#include <boost/test/auto_unit_test.hpp>

#include <sstream>
#include <fstream>

#include <boost/filesystem.hpp>

#include <coconut-tools/test-utils/test-utils.hpp>

#include "coconut/pulp/file-io/JSONSerialiser.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::file_io;

namespace /* anonymous */ {

std::ostream* testIs = nullptr;

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

struct BadStruct {
};

void serialise(Serialiser& s, const BadStruct&) {
	assert(testIs != nullptr);
	testIs->setstate(std::ios::badbit);
}

struct FailStruct {
};

void serialise(Serialiser& s, const FailStruct&) {
	assert(testIs != nullptr);
	testIs->setstate(std::ios::failbit);
}

BOOST_AUTO_TEST_SUITE(PulpFileIOJSONSerialiserTestSuite);

BOOST_FIXTURE_TEST_CASE(ThrowsIfBadPath, coconut_tools::test_utils::ResourcesDirFixture) {
	std::ofstream ofs((resourcesDir() / "no-such-dir" / "file.json").c_str());

	BOOST_CHECK_THROW(JSONSerialiser s(ofs), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(ThrowsIfFailedWrite) {
	std::ostringstream oss;
	testIs = &oss;

	JSONSerialiser s(oss);

	BOOST_CHECK_THROW(s << BadStruct(), std::runtime_error);

	oss.clear();

	BOOST_CHECK_THROW(s << FailStruct(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(SerialisesDataToJSON) {
	std::ostringstream oss;
	JSONSerialiser s(oss);

	s << BasicTypeStruct();

	BOOST_CHECK_EQUAL(
		oss.str(),
		"{\n"
		"\t\"int\": 123,\n"
		"\t\"float\": 1.3,\n"
		"\t\"string\": \"value\",\n"
		"\t\"sub-objects\": [\n"
		"\t\t{\n"
		"\t\t\t\"int\": 42\n"
		"\t\t},\n"
		"\t\t{\n"
		"\t\t\t\"int\": 42\n"
		"\t\t}\n"
		"\t]\n"
		"}"
		);
}

BOOST_AUTO_TEST_SUITE_END(/* PulpFileIOJSONSerialiserTestSuite */);

} // anonymous namespace
