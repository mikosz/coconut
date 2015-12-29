#ifndef _COCONUT_MILK_MATH_MATRIX_HPP_
#define _COCONUT_MILK_MATH_MATRIX_HPP_

#include <stdexcept>

#include <DirectXMath.h>

#include "Handedness.hpp"
#include "Vector.hpp"

namespace coconut {
namespace milk {
namespace math {

class Matrix {
public:

	static const Matrix IDENTITY;

	static Matrix translation(const Vector3d& translationVector);

	static Matrix translation(float x, float y, float z) {
		return DirectX::XMMatrixTranslation(x, y, z);
	}

	static Matrix scale(const Vector3d& scaleVector);

	static Matrix scale(float x, float y, float z) {
		return DirectX::XMMatrixScaling(x, y, z);
	}

	static Matrix rotation(const Vector3d& rotationVector);

	static Matrix rotation(float x, float y, float z) {
		return DirectX::XMMatrixRotationRollPitchYaw(x, y, z);
	}

	static Matrix orthographicProjection(
		Handedness handedness, float width, float height, float nearZ, float farZ) {
		switch (handedness) {
		case Handedness::LEFT:
			return DirectX::XMMatrixOrthographicLH(width, height, nearZ, farZ);
		case Handedness::RIGHT:
			return DirectX::XMMatrixOrthographicRH(width, height, nearZ, farZ);
		default:
			throw std::logic_error("Unknown handedness value");
		}
	}

	// TODO: change float fov to a Angle type
	static Matrix perspectiveProjection(
		Handedness handedness, float fov, float aspectRatio, float nearZ, float farZ) {
		switch (handedness) {
		case Handedness::LEFT:
			return DirectX::XMMatrixPerspectiveFovLH(fov, aspectRatio, nearZ, farZ);
		case Handedness::RIGHT:
			return DirectX::XMMatrixPerspectiveFovRH(fov, aspectRatio, nearZ, farZ);
		default:
			throw std::logic_error("Unknown handedness value");
		}
	}

	Matrix() {
	}

	Matrix(const DirectX::XMMATRIX& xmMatrix) {
		DirectX::XMStoreFloat4x4(&internal_, xmMatrix);
	}

	Matrix operator *(const Matrix& other) { // TODO: use boost operator
		return DirectX::XMMatrixMultiply(
			DirectX::XMLoadFloat4x4(&internal_),
			DirectX::XMLoadFloat4x4(&other.internal_)
			);
	}

	Matrix& operator *=(const Matrix& other) {
		DirectX::XMStoreFloat4x4(
			&internal_,
			DirectX::XMMatrixMultiply(
				DirectX::XMLoadFloat4x4(&internal_),
				DirectX::XMLoadFloat4x4(&other.internal_)
				)
			);
		return *this;
	}

	Matrix transposed() const { // TODO: cache?
		return DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&internal_));
	}

	Matrix inverted() const { // TODO: cache?
		return DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&internal_));
	}

	Vector3d extractTranslation() const { // TODO: cache?
		return Vector3d(internal_._14, internal_._24, internal_._34);
	}

	bool isIdentity() const {
		return DirectX::XMMatrixIsIdentity(DirectX::XMLoadFloat4x4(&internal_));
	}

	DirectX::XMMATRIX internal() const {
		return DirectX::XMLoadFloat4x4(&internal_);
	}

private:

	DirectX::XMFLOAT4X4 internal_;

};

} // namespace math
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_MATH_MATRIX_HPP_ */
