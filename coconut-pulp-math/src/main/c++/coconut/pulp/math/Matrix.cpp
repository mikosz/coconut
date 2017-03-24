#include "Matrix.hpp"

#include "Vector.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::math;

const Matrix Matrix::IDENTITY(DirectX::XMMatrixIdentity());

Matrix Matrix::translation(const Vec3& translationVector) {
	return Matrix::translation(translationVector.x(), translationVector.y(), translationVector.z());
}

Matrix Matrix::scale(const Vec3& scaleVector) {
	return Matrix::scale(scaleVector.x(), scaleVector.y(), scaleVector.z());
}

Matrix Matrix::rotation(const Vec3& rotationVector) {
	return Matrix::rotation(rotationVector.x(), rotationVector.y(), rotationVector.z());
}
