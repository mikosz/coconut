#include "Frustum.hpp"

#include <algorithm>
#include <cmath>

#include "coconut/milk/utils/integralValue.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::math;

Frustum::Frustum(Handedness handedness, float focalLength, float aspectRatio, float near, float far) noexcept
	: Frustum(Transform::perspectiveProjection(handedness, focalLength, aspectRatio, near, far, -1.0f))
{
}

Frustum::Frustum(const Transform& projectionTransform) noexcept :
	planes_{
		Plane(projectionTransform.matrix().row(3) + projectionTransform.matrix().row(2)),
		Plane(projectionTransform.matrix().row(3) - projectionTransform.matrix().row(2)),
		Plane(projectionTransform.matrix().row(3) + projectionTransform.matrix().row(0)),
		Plane(projectionTransform.matrix().row(3) - projectionTransform.matrix().row(0)),
		Plane(projectionTransform.matrix().row(3) + projectionTransform.matrix().row(1)),
		Plane(projectionTransform.matrix().row(3) - projectionTransform.matrix().row(1)),
	}
{
}

bool Frustum::contains(const Vec3& point) const noexcept {
	return std::all_of(planes_.begin(), planes_.end(), [&point](const auto& plane) {
			return plane.signedDistanceToPoint(point) >= 0.0f;
		});
}
