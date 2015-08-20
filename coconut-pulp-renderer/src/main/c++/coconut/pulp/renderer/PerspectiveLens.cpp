#include "PerspectiveLens.hpp"

#include <functional>

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;

PerspectiveLens::PerspectiveLens(
	milk::math::Handedness handedness,
	float fov,
	float aspectRatio,
	float nearZ,
	float farZ
	) :
	projectionMatrix_(
		milk::math::Matrix::perspectiveProjection(handedness, fov, aspectRatio, nearZ, farZ)
		)
{
}

const milk::math::Matrix& PerspectiveLens::projectionTransformation() const {
	return projectionMatrix_;
}
