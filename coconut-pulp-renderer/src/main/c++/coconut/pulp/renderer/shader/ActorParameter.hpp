#ifndef _COCONUT_PULP_RENDERER_SHADER_ACTORPARAMETER_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_ACTORPARAMETER_HPP_

#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

#include "UpdateableParameter.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class Actor;

namespace shader {

typedef UpdateableParameter<Actor> ActorParameter;

MAKE_POINTER_DEFINITIONS(ActorParameter);

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_ACTORPARAMETER_HPP_ */
