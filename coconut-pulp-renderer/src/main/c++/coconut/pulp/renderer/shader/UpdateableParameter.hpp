#ifndef _COCONUT_PULP_RENDERER_SHADER_UPDATEABLEPARAMETER_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_UPDATEABLEPARAMETER_HPP_

#include "Parameter.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace shader {

template <class SourceType>
class UpdateableParameter : public Parameter {
public:

	UpdateableParameter(
		milk::graphics::Device& graphicsDevice,
		size_t size
		) :
		Parameter(graphicsDevice, size)
	{
	}

	UpdateableParameter(const UpdateableParameter&) = delete;

	virtual ~UpdateableParameter() = default;

	void operator=(UpdateableParameter&) = delete;

	virtual void update(milk::graphics::Device& graphicsDevice, const SourceType& data) = 0;

};

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_UPDATEABLEPARAMETER_HPP_ */
