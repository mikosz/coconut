#ifndef _COCONUT_PULP_RENDERER_PERSPECTIVELENS_HPP_
#define _COCONUT_PULP_RENDERER_PERSPECTIVELENS_HPP_

#include "Lens.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class PerspectiveLens : public Lens {
public:

	PerspectiveLens(
		pulp::math::Handedness handedness,
		float fov,
		float aspectRatio,
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

#endif /* _COCONUT_PULP_RENDERER_PERSPECTIVELENS_HPP_ */
