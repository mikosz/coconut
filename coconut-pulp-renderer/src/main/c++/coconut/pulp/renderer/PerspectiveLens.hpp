#ifndef _COCONUT_PULP_RENDERER_PERSPECTIVELENS_HPP_
#define _COCONUT_PULP_RENDERER_PERSPECTIVELENS_HPP_

#include "Lens.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class PerspectiveLens : public Lens {
public:

	PerspectiveLens(
		milk::math::Handedness handedness,
		float fov,
		float aspectRatio,
		float nearZ,
		float farZ
		);

	const milk::math::Matrix& projectionTransformation() const override;

private:

	milk::math::Matrix projectionMatrix_;

};

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_PERSPECTIVELENS_HPP_ */
