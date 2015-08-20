#include "Matrix.hpp"

#include "Vector.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::math;

const Matrix Matrix::IDENTITY(DirectX::XMMatrixIdentity());

Matrix Matrix::translation(const Vector3d& translationVector) {
	return Matrix::translation(translationVector.x(), translationVector.y(), translationVector.z());
}

Matrix Matrix::scale(const Vector3d& scaleVector) {
	return Matrix::scale(scaleVector.x(), scaleVector.y(), scaleVector.z());
}

Matrix Matrix::rotation(const Vector3d& rotationVector) {
	return Matrix::rotation(rotationVector.x(), rotationVector.y(), rotationVector.z());
}
