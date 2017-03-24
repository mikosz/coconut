#include "PerspectiveLens.hpp"

#include <functional>

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;

PerspectiveLens::PerspectiveLens(
	pulp::math::Handedness handedness,
	float fov,
	float aspectRatio,
	float nearZ,
	float farZ
	) :
	projectionMatrix_(
		pulp::math::Matrix::perspectiveProjection(handedness, fov, aspectRatio, nearZ, farZ)
		)
{
}

const pulp::math::Matrix& PerspectiveLens::projectionTransformation() const {
	return projectionMatrix_;
}
