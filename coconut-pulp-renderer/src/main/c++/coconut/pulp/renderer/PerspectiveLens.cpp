#include "PerspectiveLens.hpp"

#include <functional>

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;

PerspectiveLens::PerspectiveLens(
	Handedness handedness,
	Angle fov,
	float aspectRatio,
	float nearZ,
	float farZ
	) :
	projectionMatrix_(
		Matrix4x4::perspectiveProjection(handedness, fov, aspectRatio, nearZ, farZ)
		)
{
}

const Matrix4x4& PerspectiveLens::projectionTransformation() const {
	return projectionMatrix_;
}
