#ifndef _COCONUT_PULP_RENDERER_SHADER_MATERIALPARAMETER_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_MATERIALPARAMETER_HPP_

#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

#include "../material/Material.hpp"

#include "UpdateableParameter.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace shader {

using MaterialParameter = UpdateableParameter<material::Material>;

MAKE_POINTER_DEFINITIONS(MaterialParameter);

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_MATERIALPARAMETER_HPP_ */
