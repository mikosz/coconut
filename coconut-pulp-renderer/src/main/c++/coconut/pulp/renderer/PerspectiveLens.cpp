#include "PerspectiveLens.hpp"

#include "coconut/milk/graphics/Renderer.hpp"
#include "coconut/pulp/math/Transform.hpp"

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
		Transform::perspectiveProjection(
			handedness,
			fov,
			aspectRatio,
			nearZ,
			farZ,
			milk::graphics::Renderer::NDC_NEAR
			).matrix()
		)
{
}

const Matrix4x4& PerspectiveLens::projectionTransformation() const {
	return projectionMatrix_;
}
