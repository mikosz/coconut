#include "ShaderType.hpp"

#include <type_traits>
#include <unordered_map>

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::graphics;

namespace /* anonymous */ {

// TODO: this builds on visual c++, but is probably invalid C++ (no hash function for enum class)
const std::unordered_map<ShaderType, const std::string> SHADER_TYPE_NAMES = {
	{ ShaderType::VERTEX, "VERTEX" },
	{ ShaderType::PIXEL, "PIXEL" },
};

const std::unordered_map<std::string, ShaderType> NAMES_TO_SHADER_TYPE = {
	{ "VERTEX", ShaderType::VERTEX },
	{ "PIXEL", ShaderType::PIXEL },
};

} // anonymous namespace

const std::string& coconut::milk::graphics::toString(ShaderType shaderType) {
	return SHADER_TYPE_NAMES.at(shaderType);
}

void coconut::milk::graphics::fromString(ShaderType& shaderType, const std::string& name) {
	shaderType = NAMES_TO_SHADER_TYPE.at(name);
}
