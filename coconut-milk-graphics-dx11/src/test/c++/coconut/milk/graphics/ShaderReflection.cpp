#include <boost/test/auto_unit_test.hpp>

#include <fstream>
#include <string>
#include <sstream>

#include "coconut/milk/graphics/ConstantBuffer.hpp"
#include "coconut/milk/graphics/ShaderReflection.hpp"
#include "coconut/milk/graphics/TestSuite.hpp"
#include "coconut/milk/graphics/FlexibleInputLayoutDescription.hpp"
#include "coconut/milk/graphics/InputLayout.hpp"
#include "coconut/milk/graphics/Shader.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::graphics;

namespace /* anonymous */ {

std::string readBinaryData(const std::string& path) {
	// TODO: temp! shader object needs to be copied in correct place
	std::ifstream ifs(path.c_str(), std::ios::in | std::ios::binary);
	std::ostringstream oss;
	oss << ifs.rdbuf();
	auto data = oss.str();

	BOOST_REQUIRE(!ifs.bad());
	BOOST_REQUIRE(!data.empty());

	return data;
}

ShaderReflection reflect(const std::string& path) {
	auto shaderData = readBinaryData(path);
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

	BOOST_REQUIRE_EQUAL(reflection.constantBuffers().size(), 2);

	{
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

	{
		const auto& buffer = reflection.constantBuffers()[1];
		BOOST_CHECK_EQUAL(buffer.size, 48);

		BOOST_REQUIRE_EQUAL(buffer.variables.size(), 1);

		BOOST_CHECK_EQUAL(buffer.variables[0].name, "fs");
		BOOST_CHECK_EQUAL(buffer.variables[0].offset, 0);
		BOOST_CHECK_EQUAL(buffer.variables[0].size, 44);

		BOOST_CHECK_EQUAL(buffer.variables[0].type.offset, 0);
		BOOST_CHECK_EQUAL(buffer.variables[0].type.scalarType, ShaderReflection::Type::ScalarType::FLOAT);
		BOOST_CHECK_EQUAL(buffer.variables[0].type.klass, ShaderReflection::Type::Class::VECTOR);
		BOOST_CHECK_EQUAL(buffer.variables[0].type.elements, 3);
	}
}

BOOST_FIXTURE_TEST_CASE(ArrayElementAlignmentIsCorrect, TestSuite) {
	using namespace std::string_literals;

	setDefaultRenderingParameters();

	const auto VERTEX_SHADER = "Debug/ArrayElementAlignment.v.cso"s;
	const auto PIXEL_SHADER = "Debug/ArrayElementAlignment.p.cso"s;

	auto vertexShaderData = readBinaryData(VERTEX_SHADER);
	auto pixelShaderData = readBinaryData(PIXEL_SHADER);
	const auto reflection = reflect(PIXEL_SHADER);

	auto layoutDesciption = std::make_unique<FlexibleInputLayoutDescription>();
	layoutDesciption->push(
		FlexibleInputLayoutDescription::Element(
			FlexibleInputLayoutDescription::ElementType::POSITION,
			0,
			FlexibleInputLayoutDescription::Format::R32G32B32A32_FLOAT,
			milk::graphics::InputLayoutDescription::SlotType::PER_VERTEX_DATA
			)
		);

	InputLayout inputLayout(std::move(layoutDesciption), renderer(), vertexShaderData.data(), vertexShaderData.size());
	VertexShader vertexShader(renderer(), vertexShaderData.data(), vertexShaderData.size());
	PixelShader pixelShader(renderer(), pixelShaderData.data(), pixelShaderData.size());

	BOOST_REQUIRE_EQUAL(reflection.constantBuffers().size(), 1);
	BOOST_REQUIRE_EQUAL(reflection.constantBuffers()[0].variables.size(), 4);
	BOOST_REQUIRE_EQUAL(reflection.constantBuffers()[0].variables[0].name, "smallStructs"s);
	BOOST_REQUIRE_EQUAL(reflection.constantBuffers()[0].variables[0].type.elements, 3);
	BOOST_REQUIRE_EQUAL(reflection.constantBuffers()[0].variables[1].name, "floats"s);
	BOOST_REQUIRE_EQUAL(reflection.constantBuffers()[0].variables[1].type.elements, 5);
	BOOST_REQUIRE_EQUAL(reflection.constantBuffers()[0].variables[2].name, "bigStructs"s);
	BOOST_REQUIRE_EQUAL(reflection.constantBuffers()[0].variables[2].type.elements, 2);
	BOOST_REQUIRE_EQUAL(reflection.constantBuffers()[0].variables[3].name, "withArray"s);
	BOOST_REQUIRE_EQUAL(reflection.constantBuffers()[0].variables[3].type.members.size(), 1);

	ConstantBuffer::Configuration cbConf;
	cbConf.size = reflection.constantBuffers()[0].size;
	cbConf.stride = 0;
	cbConf.allowCPURead = false;
	cbConf.allowGPUWrite = false;
	cbConf.allowModifications = false;

	std::vector<std::uint8_t> cbData(cbConf.size);

	const auto& smallStructs = reflection.constantBuffers()[0].variables[0];
	for (size_t i = 0; i < smallStructs.type.elements; ++i) {
		auto* ptr = cbData.data() + smallStructs.offset + (smallStructs.type.elementOffset * i);
		*reinterpret_cast<float*>(ptr) = (i + 1) * 0.01f;
	}

	const auto& floats = reflection.constantBuffers()[0].variables[1];
	for (size_t i = 0; i < floats.type.elements; ++i) {
		auto* ptr = cbData.data() + floats.offset + (floats.type.elementOffset * i);
		*reinterpret_cast<float*>(ptr) = (i + 4) * 0.01f;
	}

	const auto& bigStructs = reflection.constantBuffers()[0].variables[2];
	for (size_t i = 0; i < bigStructs.type.elements; ++i) {
		auto* ptr = cbData.data() + bigStructs.offset + (bigStructs.type.elementOffset * i);
		*reinterpret_cast<float*>(ptr) = (i + 9) * 0.01f;
	}

	const auto& withArray = reflection.constantBuffers()[0].variables[3];
	std::string memberName;
	ShaderReflection::Type memberType;
	std::tie(memberName, memberType) = withArray.type.members[0];
	BOOST_REQUIRE_EQUAL(memberName, "bigStructs"s);
	BOOST_REQUIRE_EQUAL(memberType.elements, 2);
	for (size_t i = 0; i < memberType.elements; ++i) {
		auto* ptr = cbData.data() + withArray.offset + memberType.offset + (memberType.elementOffset * i);
		*reinterpret_cast<float*>(ptr) = (i + 11) * 0.01f;
	}

	ConstantBuffer cb(renderer(), cbConf, cbData.data());

	auto& commandList = renderer().getImmediateCommandList();

	setSquareData();

	commandList.setInputLayout(inputLayout);
	commandList.setVertexShader(vertexShader);
	commandList.setPixelShader(pixelShader);
	commandList.setConstantBuffer(cb, ShaderType::PIXEL, 0);

	renderer().beginScene();
	drawSquare();
	renderer().endScene();

	BOOST_FAIL("");
}

BOOST_AUTO_TEST_SUITE_END(/* MilkGraphicsShaderReflectionTestSuite */);

} // anonymous namespace
