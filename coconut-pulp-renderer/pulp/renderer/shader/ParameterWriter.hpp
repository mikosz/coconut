#ifndef _COCONUT_PULP_RENDERER_SHADER_SHADER_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_SHADER_HPP_

#include "milk/utils/MakePointerDefinitionsMacro.hpp"
#include "ParameterId.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace shader {

class ParameterWriter {
public:

	virtual void* write(void* target, ParameterId parameterId) = 0;

protected:

	virtual ~ParameterWriter() {
	}

};

MAKE_POINTER_DEFINITIONS(ParameterWriter);

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_SHADER_HPP_ */
