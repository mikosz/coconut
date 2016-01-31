#include <boost/test/auto_unit_test.hpp>

#include <sstream>

#include "coconut/pulp/file-io/JSONSerialiser.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::file_io;

namespace /* anonymous */ {

struct BasicTypeStruct {
};

struct SubStruct {
};

void serialise(Serialiser& s, const SubStruct&) {
	s << Serialiser::labelled("int", 42);
}

void serialise(Serialiser& s, const BasicTypeStruct&) {
	s << Serialiser::labelled("int", 123);
	s << Serialiser::labelled("float", 1.3f);
	s << Serialiser::labelled("string", "value");

	std::vector<SubStruct> v(2);
	s << Serialiser::labelled("sub-objects", v);
}

BOOST_AUTO_TEST_SUITE(PulpFileIOJSONSerialiserTestSuite);

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
