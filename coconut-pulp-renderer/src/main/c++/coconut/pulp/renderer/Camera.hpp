#ifndef _COCONUT_PULP_RENDERER_CAMERA_HPP_
#define _COCONUT_PULP_RENDERER_CAMERA_HPP_

#include "coconut/pulp/math/Matrix.hpp"
#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class Camera {
public:

	virtual const pulp::math::Matrix& viewTransformation() const = 0;

	virtual const pulp::math::Vec3& position() const = 0;

protected:

	virtual ~Camera() {
	}

};

CCN_MAKE_POINTER_DEFINITIONS(Camera);

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_CAMERA_HPP_ */
