#include "PerspectiveLens.hpp"

#include "coconut/pulp/math/Transform.hpp"

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
		Transform::perspectiveProjection(handedness, fov, aspectRatio, nearZ, farZ).matrix()
		)
{
}

const Matrix4x4& PerspectiveLens::projectionTransformation() const {
	return projectionMatrix_;
}
