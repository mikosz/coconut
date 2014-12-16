#ifndef _COCONUT_PULP_RENDERER_SCENE_HPP_
#define _COCONUT_PULP_RENDERER_SCENE_HPP_

#include "milk/utils/MakePointerDefinitionsMacro.hpp"
#include "milk/graphics/Device.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class Lens;
class Model;

class Scene {
public:

	// TODO: model needs to be added to the scene in some other function
	void render(milk::graphics::Device& device, Model& model);

private:

};

MAKE_POINTER_DEFINITIONS(Scene);

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SCENE_HPP_ */
