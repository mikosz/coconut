#ifndef _COCONUT_PULP_RENDERER_MATERIAL_MATERIALLIBRARY_HPP_
#define _COCONUT_PULP_RENDERER_MATERIAL_MATERIALLIBRARY_HPP_

#include <string>
#include <unordered_map>

#include <coconut-tools/exceptions/LogicError.hpp>

#include "coconut/milk/graphics/Device.hpp"

#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace material {

class Material;
CCN_MAKE_POINTER_DEFINITIONS(Material);

class MaterialLibrary {
public:

	typedef std::string MaterialId;

	class MaterialAlreadyInLibrary : public coconut_tools::exceptions::LogicError {
	public:

		MaterialAlreadyInLibrary(const MaterialId& materialId) :
			coconut_tools::exceptions::LogicError("Material already in library: \"" + materialId + "\""),
			materialId_(materialId)
		{
		}

		const MaterialId& materialId() const {
			return materialId_;
		}

	private:

		MaterialId materialId_;

	};

	class NoSuchMaterialInLibrary : public coconut_tools::exceptions::LogicError {
	public:

		NoSuchMaterialInLibrary(const MaterialId& materialId) :
			coconut_tools::exceptions::LogicError("No such material in library: \"" + materialId + "\""),
			materialId_(materialId)
		{
		}

		const MaterialId& materialId() const {
			return materialId_;
		}

	private:

		MaterialId materialId_;

	};

	void put(const MaterialId& materialId, ConstMaterialSharedPtr material) {
		if (has(materialId)) { // TODO: if debug?
			throw MaterialAlreadyInLibrary(materialId);
		}

		materials_.insert(std::make_pair(materialId, material));
	}

	ConstMaterialSharedPtr get(const MaterialId& materialId) const {
		auto material = materials_.find(materialId);

		if (material == materials_.end()) { // TODO: if debug?
			throw NoSuchMaterialInLibrary(materialId);
		}

		return material->second;
	}

	bool has(const MaterialId& materialId) const {
		return materials_.count(materialId) != 0;
	}

private:

	typedef std::unordered_map<MaterialId, ConstMaterialSharedPtr> Materials;

	Materials materials_;

};

} // namespace material
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_MATERIAL_MATERIALLIBRARY_HPP_ */
