#include "PrimitiveTopology.hpp"

#include <type_traits>
#include <unordered_map>

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::graphics;

namespace /* anonymous */ {

// TODO: this builds on visual c++, but is probably invalid C++ (no hash function for enum class)
const std::unordered_map<PrimitiveTopology, const std::string> PRIMITIVE_TOPOLOGY_NAMES = {
	{ PrimitiveTopology::TRIANGLE_LIST, "TRIANGLE_LIST" },
	{ PrimitiveTopology::TRIANGLE_STRIP, "TRIANGLE_STRIP" },
};

const std::unordered_map<std::string, PrimitiveTopology> NAMES_TO_PRIMITIVE_TOPOLOGY = {
	{ "TRIANGLE_LIST", PrimitiveTopology::TRIANGLE_LIST },
	{ "TRIANGLE_STRIP", PrimitiveTopology::TRIANGLE_STRIP },
};

} // anonymous namespace

const std::string& coconut::milk::graphics::toString(PrimitiveTopology primitiveTopology) {
	return PRIMITIVE_TOPOLOGY_NAMES.at(primitiveTopology);
}

void coconut::milk::graphics::fromString(PrimitiveTopology& primitiveTopology, const std::string& name) {
	primitiveTopology = NAMES_TO_PRIMITIVE_TOPOLOGY.at(name);
}
