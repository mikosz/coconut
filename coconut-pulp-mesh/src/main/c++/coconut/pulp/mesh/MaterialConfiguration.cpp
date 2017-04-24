#include "MaterialConfiguration.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::mesh;

using namespace std::string_literals;

const std::string MaterialConfiguration::AMBIENT_COLOUR_PROPERTY = "ambient-colour"s;
const std::string MaterialConfiguration::DIFFUSE_COLOUR_PROPERTY = "diffuse-colour"s;
const std::string MaterialConfiguration::SPECULAR_COLOUR_PROPERTY = "specular-colour"s;
const std::string MaterialConfiguration::SPECULAR_EXPONENT_PROPERTY = "specular-exponent"s;

const std::string MaterialConfiguration::DIFFUSE_MAP_TEXTURE = "diffuse-map"s;
const std::string MaterialConfiguration::NOISE_MAP_TEXTURE = "noise-map"s;
