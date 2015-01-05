#ifndef _COCONUT_PULP_RENDERER_MODEL_HPP_
#define _COCONUT_PULP_RENDERER_MODEL_HPP_

#include <memory>
#include <unordered_map>

#include "milk/graphics/Buffer.hpp"
#include "milk/graphics/Device.hpp"

#include "milk/utils/MakePointerDefinitionsMacro.hpp"

#include "shader/Shader.hpp"
#include "RenderingContext.hpp"
#include "Material.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class Model {
public:

	struct SmoothingGroup {

		size_t firstIndex;

		size_t indexCount;

	};

	Model(milk::graphics::Device& device);

	void render(milk::graphics::Device& devices, RenderingContext renderingContext);

private:

	typedef std::unordered_multimap<MaterialSharedPtr, SmoothingGroup> SmoothingGroupsByMaterial;

	milk::graphics::Buffer vertexBuffer_;

	milk::graphics::Buffer indexBuffer_;

	SmoothingGroupsByMaterial smoothingGroupsByMaterial_;

};

MAKE_POINTER_DEFINITIONS(Model);

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_MODEL_HPP_ */
