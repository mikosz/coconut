#include <boost/test/auto_unit_test.hpp>

#include <fstream>
#include <string>
#include <sstream>

#include "coconut/milk/graphics/ShaderReflection.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::graphics;

namespace /* anonymous */ {

ShaderReflection reflect(const std::string& path) {
	// TODO: temp! shader object needs to be copied in correct place
	std::ifstream ifs(path.c_str(), std::ios::in | std::ios::binary);
	std::ostringstream oss;
	oss << ifs.rdbuf();
	auto shaderData = oss.str();

	BOOST_REQUIRE(!ifs.bad());
	BOOST_REQUIRE(!shaderData.empty());

	return ShaderReflection(shaderData.data(), shaderData.size());
}

BOOST_AUTO_TEST_SUITE(MilkGraphicsShaderReflectionTestSuite);

BOOST_AUTO_TEST_CASE(RetrievesSimpleInputParameters) {
	auto reflection = reflect("Debug/SimpleInputParameter.v.cso");

	BOOST_REQUIRE_EQUAL(reflection.inputParameters().size(), 1);

	const auto& inputParameter = reflection.inputParameters()[0];
	BOOST_CHECK_EQUAL(inputParameter.semantic, ShaderReflection::InputParameterSemantic::POSITION);
	BOOST_CHECK_EQUAL(inputParameter.semanticIndex, 0);
}

BOOST_AUTO_TEST_CASE(RetrievesComplexInputParameters) {
	auto reflection = reflect("Debug/ComplexInputParameter.v.cso");

	BOOST_REQUIRE_EQUAL(reflection.inputParameters().size(), 4);

	BOOST_CHECK_EQUAL(reflection.inputParameters()[0].semantic, ShaderReflection::InputParameterSemantic::POSITION);
	BOOST_CHECK_EQUAL(reflection.inputParameters()[0].semanticIndex, 0);

	BOOST_CHECK_EQUAL(reflection.inputParameters()[1].semantic, ShaderReflection::InputParameterSemantic::TEXCOORD);
	BOOST_CHECK_EQUAL(reflection.inputParameters()[1].semanticIndex, 0);

	BOOST_CHECK_EQUAL(reflection.inputParameters()[2].semantic, ShaderReflection::InputParameterSemantic::TEXCOORD);
	BOOST_CHECK_EQUAL(reflection.inputParameters()[2].semanticIndex, 1);

	BOOST_CHECK_EQUAL(reflection.inputParameters()[3].semantic, ShaderReflection::InputParameterSemantic::NORMAL);
	BOOST_CHECK_EQUAL(reflection.inputParameters()[3].semanticIndex, 0);
}

BOOST_AUTO_TEST_CASE(RetrievesSimpleConstantBuffers) {
	auto reflection = reflect("Debug/SimpleConstantBuffer.v.cso");

	BOOST_REQUIRE_EQUAL(reflection.constantBuffers().size(), 1);

	const auto& buffer = reflection.constantBuffers()[0];
	BOOST_CHECK_EQUAL(buffer.size, 96);

	BOOST_REQUIRE_EQUAL(buffer.variables.size(), 3);

	BOOST_CHECK_EQUAL(buffer.variables[0].name, "ui");
	BOOST_CHECK_EQUAL(buffer.variables[0].offset, 0);
	BOOST_CHECK_EQUAL(buffer.variables[0].size, 4);

	BOOST_CHECK_EQUAL(buffer.variables[1].name, "f4");
	BOOST_CHECK_EQUAL(buffer.variables[1].offset, 16);
	BOOST_CHECK_EQUAL(buffer.variables[1].size, 16);

	BOOST_CHECK_EQUAL(buffer.variables[2].name, "m4");
	BOOST_CHECK_EQUAL(buffer.variables[2].offset, 32);
	BOOST_CHECK_EQUAL(buffer.variables[2].size, 16 * 4);
}

BOOST_AUTO_TEST_CASE(RetrievesConstantBuffersWithNestedStructs) {
	auto reflection = reflect("Debug/ConstantBufferWithNestedStructs.v.cso");

	BOOST_REQUIRE_EQUAL(reflection.constantBuffers().size(), 1);

	const auto& buffer = reflection.constantBuffers()[0];
	BOOST_CHECK_EQUAL(buffer.size, 16);

	BOOST_REQUIRE_EQUAL(buffer.variables.size(), 1);

	BOOST_CHECK_EQUAL(buffer.variables[0].name, "s");
	BOOST_CHECK_EQUAL(buffer.variables[0].offset, 0);
	BOOST_CHECK_EQUAL(buffer.variables[0].size, 8);
}

BOOST_AUTO_TEST_SUITE_END(/* MilkGraphicsShaderReflectionTestSuite */);

} // anonymous namespace
