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
	BOOST_CHECK_EQUAL(inputParameter.semantic, ShaderReflection::InputParameterInfo::Semantic::POSITION);
	BOOST_CHECK_EQUAL(inputParameter.semanticIndex, 0);
	BOOST_CHECK_EQUAL(inputParameter.dataType, ShaderReflection::InputParameterInfo::DataType::UINT);
	BOOST_CHECK_EQUAL(inputParameter.elements, 4);
}

BOOST_AUTO_TEST_CASE(RetrievesComplexInputParameters) {
	auto reflection = reflect("Debug/ComplexInputParameter.v.cso");

	BOOST_REQUIRE_EQUAL(reflection.inputParameters().size(), 4);

	BOOST_CHECK_EQUAL(reflection.inputParameters()[0].semantic, ShaderReflection::InputParameterInfo::Semantic::POSITION);
	BOOST_CHECK_EQUAL(reflection.inputParameters()[0].semanticIndex, 0);
	BOOST_CHECK_EQUAL(reflection.inputParameters()[0].dataType, ShaderReflection::InputParameterInfo::DataType::FLOAT);
	BOOST_CHECK_EQUAL(reflection.inputParameters()[0].elements, 4);

	BOOST_CHECK_EQUAL(reflection.inputParameters()[1].semantic, ShaderReflection::InputParameterInfo::Semantic::TEXCOORD);
	BOOST_CHECK_EQUAL(reflection.inputParameters()[1].semanticIndex, 0);
	BOOST_CHECK_EQUAL(reflection.inputParameters()[1].dataType, ShaderReflection::InputParameterInfo::DataType::FLOAT);
	BOOST_CHECK_EQUAL(reflection.inputParameters()[1].elements, 2);

	BOOST_CHECK_EQUAL(reflection.inputParameters()[2].semantic, ShaderReflection::InputParameterInfo::Semantic::TEXCOORD);
	BOOST_CHECK_EQUAL(reflection.inputParameters()[2].semanticIndex, 1);
	BOOST_CHECK_EQUAL(reflection.inputParameters()[2].dataType, ShaderReflection::InputParameterInfo::DataType::FLOAT);
	BOOST_CHECK_EQUAL(reflection.inputParameters()[2].elements, 2);

	BOOST_CHECK_EQUAL(reflection.inputParameters()[3].semantic, ShaderReflection::InputParameterInfo::Semantic::NORMAL);
	BOOST_CHECK_EQUAL(reflection.inputParameters()[3].semanticIndex, 0);
	BOOST_CHECK_EQUAL(reflection.inputParameters()[3].dataType, ShaderReflection::InputParameterInfo::DataType::FLOAT);
	BOOST_CHECK_EQUAL(reflection.inputParameters()[3].elements, 3);
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

	BOOST_CHECK_EQUAL(buffer.variables[0].type.name, "S");
	BOOST_CHECK_EQUAL(buffer.variables[0].type.offset, 0);
	BOOST_CHECK_EQUAL(buffer.variables[0].type.scalarType, ShaderReflection::Type::ScalarType::VOID);
	BOOST_CHECK_EQUAL(buffer.variables[0].type.klass, ShaderReflection::Type::Class::STRUCT);
	BOOST_CHECK_EQUAL(buffer.variables[0].type.elements, 0);
	BOOST_CHECK_EQUAL(buffer.variables[0].type.members.size(), 2);

	{
		std::string name;
		ShaderReflection::Type type;
		std::tie(name, type) = buffer.variables[0].type.members[0];
		BOOST_CHECK_EQUAL(name, "s");

		BOOST_CHECK_EQUAL(type.name, "Sub");
		BOOST_CHECK_EQUAL(type.offset, 0);
		BOOST_CHECK_EQUAL(type.scalarType, ShaderReflection::Type::ScalarType::VOID);
		BOOST_CHECK_EQUAL(type.klass, ShaderReflection::Type::Class::STRUCT);
		BOOST_CHECK_EQUAL(type.elements, 0);
		BOOST_CHECK_EQUAL(type.members.size(), 1);

		{
			std::string subName;
			ShaderReflection::Type subType;
			std::tie(subName, subType) = type.members[0];
			BOOST_CHECK_EQUAL(subName, "ui");

			BOOST_CHECK_EQUAL(subType.offset, 0);
			BOOST_CHECK_EQUAL(subType.scalarType, ShaderReflection::Type::ScalarType::UINT);
			BOOST_CHECK_EQUAL(subType.klass, ShaderReflection::Type::Class::SCALAR);
			BOOST_CHECK_EQUAL(subType.elements, 0);
			BOOST_CHECK_EQUAL(subType.members.size(), 0);
		}
	}

	{
		std::string name;
		ShaderReflection::Type type;
		std::tie(name, type) = buffer.variables[0].type.members[1];
		BOOST_CHECK_EQUAL(name, "f");

		BOOST_CHECK_EQUAL(type.offset, 4);
		BOOST_CHECK_EQUAL(type.scalarType, ShaderReflection::Type::ScalarType::FLOAT);
		BOOST_CHECK_EQUAL(type.klass, ShaderReflection::Type::Class::SCALAR);
		BOOST_CHECK_EQUAL(type.elements, 0);
		BOOST_CHECK_EQUAL(type.members.size(), 0);
	}
}

BOOST_AUTO_TEST_CASE(RetrievesConstantBuffersWithArrays) {
	auto reflection = reflect("Debug/ConstantBufferWithArrays.v.cso");

	BOOST_REQUIRE_EQUAL(reflection.constantBuffers().size(), 1);

	const auto& buffer = reflection.constantBuffers()[0];
	BOOST_CHECK_EQUAL(buffer.size, 144);

	BOOST_REQUIRE_EQUAL(buffer.variables.size(), 2);

	BOOST_CHECK_EQUAL(buffer.variables[0].name, "s");
	BOOST_CHECK_EQUAL(buffer.variables[0].offset, 0);
	BOOST_CHECK_EQUAL(buffer.variables[0].size, 128);

	BOOST_CHECK_EQUAL(buffer.variables[0].type.name, "S");
	BOOST_CHECK_EQUAL(buffer.variables[0].type.offset, 0);
	BOOST_CHECK_EQUAL(buffer.variables[0].type.scalarType, ShaderReflection::Type::ScalarType::VOID);
	BOOST_CHECK_EQUAL(buffer.variables[0].type.klass, ShaderReflection::Type::Class::STRUCT);
	BOOST_CHECK_EQUAL(buffer.variables[0].type.elements, 2);
	BOOST_CHECK_EQUAL(buffer.variables[0].type.members.size(), 1);

	BOOST_CHECK_EQUAL(buffer.variables[1].name, "f");
	BOOST_CHECK_EQUAL(buffer.variables[1].offset, 128);
	BOOST_CHECK_EQUAL(buffer.variables[1].size, 4);

	BOOST_CHECK_EQUAL(buffer.variables[1].type.name, "float");
	BOOST_CHECK_EQUAL(buffer.variables[1].type.offset, 0);
	BOOST_CHECK_EQUAL(buffer.variables[1].type.scalarType, ShaderReflection::Type::ScalarType::FLOAT);
	BOOST_CHECK_EQUAL(buffer.variables[1].type.klass, ShaderReflection::Type::Class::SCALAR);
	BOOST_CHECK_EQUAL(buffer.variables[1].type.elements, 0);
	BOOST_CHECK_EQUAL(buffer.variables[1].type.members.size(), 0);
}

BOOST_AUTO_TEST_SUITE_END(/* MilkGraphicsShaderReflectionTestSuite */);

} // anonymous namespace
