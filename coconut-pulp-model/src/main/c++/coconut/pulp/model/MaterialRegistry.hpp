#ifndef _COCONUT_PULP_RENDERER_MATERIALMANAGER_HPP_
#define _COCONUT_PULP_RENDERER_MATERIALMANAGER_HPP_

#include <memory>
#include <unordered_map>

#include <coconut-tools/exceptions/RuntimeError.hpp>

#include "Material.hpp"

namespace coconut {
namespace pulp {
namespace model {

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

class MaterialRegistry {
public:

	void add(std::string name, Material material);

	const Material& get(const std::string& name) const;

private:

	using Materials = std::unordered_map<std::string, Material>;

	Materials materials_;

};

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_MATERIALMANAGER_HPP_ */
