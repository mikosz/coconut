#ifndef _COCONUT_PULP_RENDERER_SHADER_OBSERVERAWARESHADER_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_OBSERVERAWARESHADER_HPP_

#include "../Camera.hpp"
#include "../Lens.hpp"
#include "ParameterWriter.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace shader {

class ObserverAwareParameterWriter : public ParameterWriter {
public:

	ObserverAwareParameterWriter(
		CameraSharedPtr camera,
		LensSharedPtr lens
		) :
		camera_(camera),
		lens_(lens)
	{
	}

	void* write(void* target, ParameterId parameterId) override;

private:

	CameraSharedPtr camera_;

	LensSharedPtr lens_;
	
};

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_OBSERVERAWARESHADER_HPP_ */
