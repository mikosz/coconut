#include "OrthographicLens.hpp"

#include <functional>

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;

OrthographicLens::OrthographicLens(
	milk::math::Handedness handedness,
	float viewWidth,
	float viewHeight,
	float nearZ,
	float farZ
	) :
	projectionMatrix_(
		milk::math::Matrix::orthographicProjection(handedness, viewWidth, viewHeight, nearZ, farZ)
		)
{
}

const milk::math::Matrix& OrthographicLens::projectionTransformation() const {
	return projectionMatrix_;
}
