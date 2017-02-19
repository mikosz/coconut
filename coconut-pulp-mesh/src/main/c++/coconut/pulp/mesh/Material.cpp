#include "Material.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::mesh;

Material& Material::set(const std::string& property, Primitive value) {
	properties_.emplace(property, std::move(value));
	return *this;
}

const Primitive& Material::get(const std::string& property) const {
	auto it = properties_.find(property);

	if (it == properties_.end()) {
		throw NoSuchProperty(property);
	}

	return it->second;
}

Material& Material::addTexture(
	std::string textureName,
	milk::fs::AbsolutePath path,
	milk::graphics::Sampler::Configuration samplerConfiguration
	)
{
	textures_.emplace(textureName, std::make_tuple(std::move(path), std::move(samplerConfiguration)));
	return *this;
}
