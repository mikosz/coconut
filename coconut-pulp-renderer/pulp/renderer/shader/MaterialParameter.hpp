#ifndef _COCONUT_PULP_RENDERER_SHADER_MATERIALPARAMETER_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_MATERIALPARAMETER_HPP_

#include "milk/utils/MakePointerDefinitionsMacro.hpp"

#include "UpdateableParameter.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class Material;

namespace shader {

typedef UpdateableParameter<Material> MaterialParameter;

MAKE_POINTER_DEFINITIONS(MaterialParameter);

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_MATERIALPARAMETER_HPP_ */
