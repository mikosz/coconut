#ifndef _COCONUT_PULP_RENDERER_MATERIALMANAGER_HPP_
#define _COCONUT_PULP_RENDERER_MATERIALMANAGER_HPP_

#include <memory>
#include <unordered_map>

#include <coconut-tools/exceptions/RuntimeError.hpp>

#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class MaterialAlreadyRegistered : public coconut_tools::exceptions::RuntimeError {
public:

	MaterialAlreadyRegistered(const std::string& name) :
		coconut_tools::exceptions::RuntimeError("Material " + name + " is already registered")
	{
	}

};

class MaterialNotRegistered : public coconut_tools::exceptions::RuntimeError {
public:

	MaterialNotRegistered(const std::string& name) :
		coconut_tools::exceptions::RuntimeError("Material " + name + " is not registered")
	{
	}

};

class Material;
CCN_MAKE_POINTER_DEFINITIONS(Material);

class MaterialManager {
public:

	void registerMaterial(std::string name, MaterialSharedPtr material) {
		if (materials_.count(name) != 0) {
			throw MaterialAlreadyRegistered(name);
		}
		materials_.emplace(name, material);
	}

	MaterialSharedPtr get(const std::string& name) const {
		auto material = materials_.find(name);
		if (material == materials_.end()) {
			throw MaterialNotRegistered(name);
		}
		return material->second;
	}

private:

	using Materials = std::unordered_map<std::string, MaterialSharedPtr>;

	Materials materials_;

};

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_MATERIALMANAGER_HPP_ */
