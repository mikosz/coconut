#include "MaterialConfiguration.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::mesh;

using namespace std::string_literals;

const std::string MaterialConfiguration::AMBIENT_COLOUR_PROPERTY = "ambientColour"s;
const std::string MaterialConfiguration::DIFFUSE_COLOUR_PROPERTY = "diffuseColour"s;
const std::string MaterialConfiguration::SPECULAR_COLOUR_PROPERTY = "specularColour"s;
const std::string MaterialConfiguration::SPECULAR_EXPONENT_PROPERTY = "specularExponent"s;

const std::string MaterialConfiguration::DIFFUSE_MAP_TEXTURE = "diffuseMap"s;
const std::string MaterialConfiguration::NOISE_MAP_TEXTURE = "noiseMap"s;
