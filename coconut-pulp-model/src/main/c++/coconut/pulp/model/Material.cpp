#include "Material.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::model;

void Material::set(const std::string& property, boost::any value) {
	properties_.emplace(property, std::move(value));
}

const boost::any& Material::get(const std::string& property) const {
	auto it = properties_.find(property);

	if (it == properties_.end()) {
		throw NoSuchProperty(property);
	}

	return it->second;
}
