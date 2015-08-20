#ifndef _COCONUT_PULP_RENDERER_SHADER_SCENEPARAMETER_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_SCENEPARAMETER_HPP_

#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

#include "UpdateableParameter.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class Scene;

namespace shader {

typedef UpdateableParameter<Scene> SceneParameter;

MAKE_POINTER_DEFINITIONS(SceneParameter);

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_SCENEPARAMETER_HPP_ */
