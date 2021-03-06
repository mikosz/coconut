#ifndef _COCONUT_PULP_RENDERER_STATICCAMERA_HPP_
#define _COCONUT_PULP_RENDERER_STATICCAMERA_HPP_

#include "Camera.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class StaticCamera : public Camera {
public:

	const pulp::math::Matrix& viewTransformation() const override {
		return pulp::math::Matrix::IDENTITY;
	}

};

CT_MAKE_POINTER_DEFINITIONS(StaticCamera);

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_STATICCAMERA_HPP_ */
