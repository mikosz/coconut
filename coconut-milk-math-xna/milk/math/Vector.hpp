#ifndef _COCONUT_MILK_MATH_VECTOR_HPP_
#define _COCONUT_MILK_MATH_VECTOR_HPP_

#include <functional>

#include <DirectXMath.h>

#include "Matrix.hpp"

namespace coconut {
namespace milk {
namespace math {

class Vector4d {
public:

	Vector4d() {
	}

	Vector4d(const DirectX::XMVECTOR& xmVector) {
		DirectX::XMStoreFloat4(&internal_, xmVector);
	}

	Vector4d(float x, float y, float z, float w) :
		internal_(x, y, z, w)
	{
	}

	Vector4d& operator*=(const Matrix& matrix) {
		DirectX::XMStoreFloat4(
			&internal_,
			DirectX::XMVector4Transform(
				DirectX::XMLoadFloat4(&internal_),
				matrix.internal()
				)
			);
		return *this;
	}

	const Vector4d operator*(const Matrix& matrix) const {
		Vector4d result = *this;
		result *= matrix;
		return result;
	}

	Vector4d& operator*=(float scalar) {
		DirectX::XMStoreFloat4(
			&internal_,
			DirectX::XMVectorScale(
				DirectX::XMLoadFloat4(&internal_),
				scalar
				)
			);
		return *this;
	}

	const Vector4d operator*(float scalar) const {
		Vector4d result = *this;
		result *= scalar;
		return result;
	}

	const Vector4d operator-() const {
		return *this * -1.0f;
	}

	float dot(const Vector4d& other) const {
		DirectX::XMVECTOR result = DirectX::XMVector3Dot(
			DirectX::XMLoadFloat4(&internal_),
			DirectX::XMLoadFloat4(&other.internal_)
			);
		return DirectX::XMVectorGetX(result);
	}

	float& x() {
		return internal_.x;
	}

	float x() const {
		return internal_.x;
	}

	float& y() {
		return internal_.y;
	}

	float y() const {
		return internal_.y;
	}

	float& z() {
		return internal_.z;
	}

	float z() const {
		return internal_.z;
	}

	float& w() {
		return internal_.w;
	}

	float w() const {
		return internal_.w;
	}

	const DirectX::XMVECTOR internal() const {
		return DirectX::XMLoadFloat4(&internal_);
	}

protected:

	DirectX::XMFLOAT4& internal() {
		return internal_;
	}

private:

	DirectX::XMFLOAT4 internal_;

};

class Vector3d : private Vector4d {
public:

	Vector3d() {
	}

	Vector3d(const DirectX::XMVECTOR& other) :
		Vector4d(other)
	{
	}

	Vector3d(float x, float y, float z) :
		Vector4d(x, y, z, 1.0f)
	{
	}

	Vector3d& operator*=(const Matrix& matrix) {
		Vector4d::operator*=(matrix);
		return *this;
	}

	const Vector3d operator*(const Matrix& matrix) const {
		Vector3d result = *this;
		result *= matrix;
		return result;
	}

	Vector3d& operator*=(float scalar) {
		Vector4d::operator*=(scalar);
		return *this;
	}

	const Vector3d operator*(float scalar) const {
		Vector3d result = *this;
		result *= scalar;
		return result;
	}

	Vector3d operator-() const {
		return *this * -1.0f;
	}

	float dot(const Vector3d& other) const {
		return Vector4d::dot(other);
	}

	Vector3d cross(const Vector3d& other) const {
		return DirectX::XMVector3Cross(internal(), other.internal());
	}

	using Vector4d::x;

	using Vector4d::y;

	using Vector4d::z;

};

class Vector2d : private Vector4d {
public:

	Vector2d() {
	}

	Vector2d(const DirectX::XMVECTOR& other) :
		Vector4d(other)
	{
	}

	Vector2d(float x, float y) :
		Vector4d(x, y, 1.0f, 1.0f)
	{
	}

	Vector2d& operator*=(const Matrix& matrix) {
		Vector4d::operator*=(matrix);
		return *this;
	}

	const Vector2d operator*(const Matrix& matrix) const {
		Vector2d result = *this;
		result *= matrix;
		return result;
	}

	Vector2d& operator*=(float scalar) {
		Vector4d::operator*=(scalar);
		return *this;
	}

	const Vector2d operator*(float scalar) const {
		Vector2d result = *this;
		result *= scalar;
		return result;
	}

	Vector2d operator-() const {
		return *this * -1.0f;
	}

	float dot(const Vector2d& other) const {
		return Vector4d::dot(other);
	}

	using Vector4d::x;

	using Vector4d::y;

};

}
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_MATH_VECTOR_HPP_ */
