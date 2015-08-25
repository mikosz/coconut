#ifndef _COCONUT_PULP_RENDERER_MATERIALFACTORY_HPP_
#define _COCONUT_PULP_RENDERER_MATERIALFACTORY_HPP_

#include <string>

#include "coconut/milk/graphics/Device.hpp"

#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class Material;
MAKE_POINTER_DEFINITIONS(Material);

class MaterialFactory {
public:

	typedef std::string MaterialId;

	MaterialSharedPtr createMaterial(milk::graphics::Device& graphicsDevice, const MaterialId& materialId);

};

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_MATERIALFACTORY_HPP_ */
