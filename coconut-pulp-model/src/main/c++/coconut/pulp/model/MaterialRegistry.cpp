#include "MaterialRegistry.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::model;

void MaterialRegistry::add(std::string name, Material material) {
		if (materials_.count(name) != 0) {
			throw MaterialAlreadyRegistered(name);
		}
		materials_.emplace(std::move(name), std::move(material));
}

const Material& MaterialRegistry::get(const std::string& name) const {
	auto it = materials_.find(name);
	if (it == materials_.end()) {
		throw MaterialNotRegistered(name);
	}
	return it->second;
}
