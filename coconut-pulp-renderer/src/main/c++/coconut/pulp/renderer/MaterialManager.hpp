#ifndef _COCONUT_PULP_RENDERER_MATERIALMANAGER_HPP_
#define _COCONUT_PULP_RENDERER_MATERIALMANAGER_HPP_

#include <memory>
#include <unordered_map>

#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class Material;
CCN_MAKE_POINTER_DEFINITIONS(Material);

class MaterialManager {
public:

	void registerMaterial(std::string name, MaterialSharedPtr material) {
		if (materials_.count(name) != 0) {
		}
		materials_.emplace(name, material);
	}

	MaterialSharedPtr get(const std::string& name) {
		auto material = materials_.find(name);
		if (material == materials_.end()) {
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
