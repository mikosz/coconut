#ifndef _COCONUT_PULP_RENDERER_OBSERVERAWARESHADER_HPP_
#define _COCONUT_PULP_RENDERER_OBSERVERAWARESHADER_HPP_

#include "Shader.hpp"
#include "Camera.hpp"
#include "Lens.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class ObserverAwareShader : public Shader {
public:

	ObserverAwareShader(
		milk::graphics::ShaderSharedPtr wrappedShader,
		CameraSharedPtr camera,
		LensSharedPtr lens
		) :
		Shader(wrappedShader),
		camera_(camera),
		lens_(lens)
	{
	}

protected:

	CameraSharedPtr camera_;

	LensSharedPtr lens_;
	
	void update(milk::graphics::Device& device) override;

};

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_OBSERVERAWARESHADER_HPP_ */
