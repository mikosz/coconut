#include "Frustum.hpp"

#include <algorithm>
#include <cmath>

#include "coconut/milk/utils/integralValue.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::math;

Frustum::Frustum(float focalLength, float aspectRatio, float near, float far) noexcept {
	using milk::utils::integralValue;

	const auto sideDivisor = std::sqrt(focalLength * focalLength + 1);
	const auto facingDivisor = std::sqrt(focalLength * focalLength + aspectRatio * aspectRatio);

	planes_[integralValue(Side::NEAR)] = Plane({ 0.0f, 0.0f, -1.0f }, -near);
	planes_[integralValue(Side::FAR)] = Plane({ 0.0f, 0.0f, 1.0f }, far);
	planes_[integralValue(Side::LEFT)] = Plane({ focalLength / sideDivisor, 0.0f, -1.0f / sideDivisor }, 0.0f);
	planes_[integralValue(Side::RIGHT)] = Plane({ -focalLength / sideDivisor, 0.0f, -1.0f / sideDivisor }, 0.0f);
	planes_[integralValue(Side::BOTTOM)] = Plane({ 0.0f, focalLength / facingDivisor, -aspectRatio / facingDivisor }, 0.0f);
	planes_[integralValue(Side::TOP)] = Plane({ 0.0f, -focalLength / facingDivisor, -aspectRatio / facingDivisor }, 0.0f);
}

Frustum::Frustum(const Transform& projectionTransform) noexcept :
	planes_{
		Plane(projectionTransform.matrix().column(3) + projectionTransform.matrix().column(2)),
		Plane(projectionTransform.matrix().column(3) - projectionTransform.matrix().column(2)),
		Plane(projectionTransform.matrix().column(3) + projectionTransform.matrix().column(0)),
		Plane(projectionTransform.matrix().column(3) - projectionTransform.matrix().column(0)),
		Plane(projectionTransform.matrix().column(3) + projectionTransform.matrix().column(1)),
		Plane(projectionTransform.matrix().column(3) - projectionTransform.matrix().column(1)),
	}
{
}

bool Frustum::contains(const Vec3& point) const noexcept {
	return std::all_of(planes_.begin(), planes_.end(), [&point](const auto& plane) {
			return plane.signedDistanceToPoint(point) >= 0.0f;
		});
}
