#include "Transform.hpp"

#include "ScalarEqual.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::math;

Transform Transform::orthographicProjection(
	Handedness handedness,
	float left,
	float right,
	float top,
	float bottom,
	float near,
	float far
	) noexcept
{
	auto matrix = Matrix4x4();
	matrix[0][0] = 2.0f / (right - left);
	matrix[1][1] = 2.0f / (top - bottom);
	if (handedness == Handedness::RIGHT) {
		matrix[2][2] = -2.0f / (far - near);
	} else {
		matrix[2][2] = 2.0f / (far - near);
	}
	matrix[0][3] = -(right + left) / (right - left);
	matrix[1][3] = -(top + bottom) / (top - bottom);
	matrix[2][3] = -(far + near) / (far - near);
	matrix[3][3] = 1.0f;

	return Transform(matrix);
}

Transform Transform::perspectiveProjection(
	Handedness handedness,
	Angle verticalFOV,
	float aspectRatio,
	float near,
	float far
	) noexcept
{
	const auto scale = 1.0f / std::tan(verticalFOV.radians() / 2.0f);

	auto matrix = Matrix4x4();
	matrix[0][0] = (1.0f / aspectRatio) * scale;
	matrix[1][1] = scale;
	if (handedness == Handedness::RIGHT) {
		matrix[2][2] = -far / (far - near);
		matrix[2][3] = -1.0f;
	} else {
		matrix[2][2] = far / (far - near);
		matrix[2][3] = 1.0f;
	}
	matrix[3][2] = -far * near / (far - near);

	return Transform(matrix);
}

Transform Transform::translation(const Vec3& vector) noexcept {
	auto matrix = Matrix4x4::IDENTITY;
	matrix[3][0] = vector.x();
	matrix[3][1] = vector.y();
	matrix[3][2] = vector.z();

	return Transform(matrix);
}

Transform Transform::scale(const Vec3& by) noexcept {
	auto matrix = Matrix4x4::IDENTITY;
	matrix[0][0] = by.x();
	matrix[1][1] = by.y();
	matrix[2][2] = by.z();

	return Transform(matrix);
}

Transform Transform::rotation(const Vec3& around, Angle by) noexcept {
	assert(ScalarEqual<float>()(around.length(), 1.0f));

	const auto x = around.x();
	const auto y = around.y();
	const auto z = around.z();
	const auto xSq = x * x;
	const auto ySq = y * y;
	const auto zSq = z * z;
	const auto xy = x * y;
	const auto xz = x * z;
	const auto yz = y * z;
	const auto cos = std::cos(by.radians());
	const auto sin = std::sin(by.radians());

	auto matrix = Matrix4x4();
	matrix[0][0] = xSq + (1.0f - xSq) * cos;
	matrix[0][1] = xy * (1.0f - cos) + z * sin;
	matrix[0][2] = xz * (1.0f - cos) - y * sin;
	matrix[1][0] = xy * (1.0f - cos) - z * sin;
	matrix[1][1] = ySq + (1.0f - ySq) * cos;
	matrix[1][2] = yz * (1.0f - cos) + x * sin;
	matrix[2][0] = xz * (1.0f - cos) + y * sin;
	matrix[2][1] = yz * (1.0f - cos) - x * sin;
	matrix[2][2] = zSq + (1.0f - zSq) * cos;
	matrix[3][3] = 1.0f;

	return Transform(matrix);
}

Vec4 Transform::apply(const Vec4& vector) const noexcept {
	return matrix_ * vector;
}

Transform& Transform::append(const Transform& next) noexcept {
	matrix_ *= next.matrix_;
	return *this;
}

Transform Transform::then(const Transform& next) const noexcept {
	return Transform(matrix_ * next.matrix_);
}
