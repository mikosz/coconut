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

	/* operator const DirectX::XMVECTOR&() {
		return DirectX::XMLoadFloat4(&internal_);
	} */

	Vector4d operator *(const Matrix& matrix) {
		return DirectX::XMVector4Transform(
			DirectX::XMLoadFloat4(&internal_),
			matrix.internal()
			);
	}

	Vector4d& operator *=(const Matrix& matrix) {
		DirectX::XMStoreFloat4(
			&internal_,
			DirectX::XMVector4Transform(
				DirectX::XMLoadFloat4(&internal_),
				matrix.internal()
				)
			);
		return *this;
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

	/* operator const DirectX::XMVECTOR&() {
		return internal();
	} */

	Vector3d operator *(const Matrix& matrix) const {
		return DirectX::XMVector3Transform(internal(),matrix.internal());
	}

	Vector3d& operator *=(const Matrix& matrix) {
		DirectX::XMStoreFloat4(
			&internal(),
			DirectX::XMVector3Transform(const_cast<const Vector3d*>(this)->internal(), matrix.internal())
			);
		return *this;
	}

	float dot(const Vector3d& other) const {
		DirectX::XMVECTOR result = DirectX::XMVector3Dot(internal(), other.internal());
		return DirectX::XMVectorGetX(result);
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

	/* operator const DirectX::XMVECTOR&() {
		return internal();
	} */

	Vector2d operator *(const Matrix& matrix) const {
		return DirectX::XMVector2Transform(internal(), matrix.internal());
	}

	Vector2d& operator *=(const Matrix& matrix) {
		DirectX::XMStoreFloat4(
			&internal(),
			DirectX::XMVector2Transform(const_cast<const Vector2d*>(this)->internal(), matrix.internal())
			);
		return *this;
	}

	float dot(const Vector2d& other) const {
		DirectX::XMVECTOR result = DirectX::XMVector2Dot(internal(), other.internal());
		return DirectX::XMVectorGetX(result);
	}

	using Vector4d::x;

	using Vector4d::y;

};

}
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_MATH_VECTOR_HPP_ */
