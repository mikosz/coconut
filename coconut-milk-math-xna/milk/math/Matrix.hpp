#ifndef _COCONUT_MILK_MATH_MATRIX_HPP_
#define _COCONUT_MILK_MATH_MATRIX_HPP_

#include <stdexcept>

#include <DirectXMath.h>

#include "Handedness.hpp"

namespace coconut {
namespace milk {
namespace math {

class Vector3d;

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

	static Matrix rotation(float roll, float pitch, float yaw) {
		return DirectX::XMMatrixRotationRollPitchYaw(roll, pitch, yaw);
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

	Matrix() {
	}

	Matrix(const DirectX::XMMATRIX& xmMatrix) :
		internal_(xmMatrix)
	{
	}

	Matrix operator *(const Matrix& other) {
		return DirectX::XMMatrixMultiply(internal_, other.internal_);
	}

	Matrix& operator *=(const Matrix& other) {
		internal_ = DirectX::XMMatrixMultiply(internal_, other.internal_);
		return *this;
	}

	Matrix transposed() const {
		return DirectX::XMMatrixTranspose(internal_);
	}

	const DirectX::XMMATRIX& internal() const {
		return internal_;
	}

private:

	DirectX::XMMATRIX internal_;

};

} // namespace math
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_MATH_MATRIX_HPP_ */
