#include "OrthographicLens.hpp"

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
	projectionMatrix_(Matrix4x4::orthographicProjection(
		handedness,
		-viewWidth * 0.5f,
		viewWidth * 0.5f, 
		-viewHeight * 0.5f,
		viewHeight * 0.5f,
		nearZ,
		farZ
		))
{
}

const Matrix4x4& OrthographicLens::projectionTransformation() const {
	return projectionMatrix_;
}
