#ifndef _COCONUT_MILK_MATH_MATRIX_HPP_
#define _COCONUT_MILK_MATH_MATRIX_HPP_

#include <DirectXMath.h>

namespace coconut {
namespace milk {
namespace math {

class Matrix {
public:

	static Matrix translation(float x, float y, float z) {
		return DirectX::XMMatrixTranslation(x, y, z);
	}

	static Matrix scale(float x, float y, float z) {
		return DirectX::XMMatrixScaling(x, y, z);
	}

	static Matrix rotation(float roll, float pitch, float yaw) {
		return DirectX::XMMatrixRotationRollPitchYaw(roll, pitch, yaw);
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

	DirectX::XMMATRIX transposed() const {
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
