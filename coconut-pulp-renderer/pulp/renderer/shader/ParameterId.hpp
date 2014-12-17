#ifndef _COCONUT_PULP_RENDERER_SHADER_PARAMETERID_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_PARAMETERID_HPP_

namespace coconut {
namespace pulp {
namespace renderer {
namespace shader {

enum class ParameterId {
	WORLD_MATRIX,
	PROJECTION_MATRIX,
	VIEW_MATRIX,
};

size_t size(ParameterId id);

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_PARAMETERID_HPP_ */
