#include "OrthographicLens.hpp"

#include "coconut/milk/graphics/Renderer.hpp"
#include "coconut/pulp/math/Transform.hpp"

#include <functional>

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;

OrthographicLens::OrthographicLens(
	pulp::math::Handedness handedness,
	float viewWidth,
	float viewHeight,
	float nearZ,
	float farZ
	) :
	projectionMatrix_(Transform::orthographicProjection(
		handedness,
		-viewWidth * 0.5f,
		viewWidth * 0.5f, 
		-viewHeight * 0.5f,
		viewHeight * 0.5f,
		nearZ,
		farZ,
		milk::graphics::Renderer::NDC_NEAR
		).matrix())
{
}

const Matrix4x4& OrthographicLens::projectionTransformation() const {
	return projectionMatrix_;
}
