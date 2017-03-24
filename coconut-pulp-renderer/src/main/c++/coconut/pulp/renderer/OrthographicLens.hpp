#ifndef _COCONUT_PULP_RENDERER_ORTHOGRAPHIC_HPP_
#define _COCONUT_PULP_RENDERER_ORTHOGRAPHIC_HPP_

#include "Lens.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class OrthographicLens : public Lens {
public:

	OrthographicLens(
		pulp::math::Handedness handedness,
		float viewWidth,
		float viewHeight,
		float nearZ,
		float farZ
		);

	const pulp::math::Matrix& projectionTransformation() const override;

private:

	pulp::math::Matrix projectionMatrix_;

};

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_ORTHOGRAPHIC_HPP_ */
