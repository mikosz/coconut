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

	struct Proxy {
	public:

		operator float() {
			return value_;
		}

		void operator =(float value) {
			value_ = value;
		}

		~Proxy() {
			vector_ = callback_(vector_, value_);
		}

	private:

		typedef std::function<DirectX::XMVECTOR(DirectX::XMVECTOR, float)> SetCallback;

		Proxy(SetCallback callback, DirectX::XMVECTOR& vector, float value) :
			callback_(callback),
			vector_(vector),
			value_(value)
		{
		}

		// Private copy constructor so that the client doesn't copy this instance
		Proxy(const Proxy& other) :
			callback_(other.callback_),
			vector_(other.vector_),
			value_(other.value_)
		{
		}

		Proxy& operator=(const Proxy& other);

		SetCallback callback_;

		DirectX::XMVECTOR& vector_;

		float value_;

		friend class Vector2d;

		friend class Vector3d;

		friend class Vector4d;

	};

	Vector4d() {
	}

	Vector4d(const DirectX::XMVECTOR& xmVector) :
		internal_(xmVector)
	{
	}

	Vector4d(float x, float y, float z, float w) :
		internal_(DirectX::XMVectorSet(x, y, z, w))
	{
	}

	operator const DirectX::XMVECTOR&() {
		return internal_;
	}

	Vector4d operator *(const Matrix& matrix) {
		return DirectX::XMVector4Transform(internal_, matrix.internal());
	}

	Vector4d& operator *=(const Matrix& matrix) {
		internal_ = DirectX::XMVector4Transform(internal_, matrix.internal());
		return *this;
	}

	float dot(const Vector4d& other) const {
		DirectX::XMVECTOR result = DirectX::XMVector3Dot(internal_, other.internal_);
		return DirectX::XMVectorGetX(result);
	}

	Proxy x() {
		return Proxy(&DirectX::XMVectorSetX, internal_, DirectX::XMVectorGetX(internal_));
	}

	float x() const {
		return DirectX::XMVectorGetX(internal_);
	}

	Proxy y() {
		return Proxy(&DirectX::XMVectorSetY, internal_, DirectX::XMVectorGetY(internal_));
	}

	float y() const {
		return DirectX::XMVectorGetY(internal_);
	}

	Proxy z() {
		return Proxy(&DirectX::XMVectorSetZ, internal_, DirectX::XMVectorGetZ(internal_));
	}

	float z() const {
		return DirectX::XMVectorGetZ(internal_);
	}

	Proxy w() {
		return Proxy(&DirectX::XMVectorSetW, internal_, DirectX::XMVectorGetW(internal_));
	}

	float w() const {
		return DirectX::XMVectorGetW(internal_);
	}

	const DirectX::XMVECTOR& internal() const {
		return internal_;
	}

protected:

	DirectX::XMVECTOR& internal() {
		return internal_;
	}

private:

	DirectX::XMVECTOR internal_;

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

	operator const DirectX::XMVECTOR&() {
		return internal();
	}

	Vector3d operator *(const Matrix& matrix) {
		return DirectX::XMVector3Transform(internal(), matrix.internal());
	}

	Vector3d& operator *=(const Matrix& matrix) {
		internal() = DirectX::XMVector3Transform(internal(), matrix.internal());
		return *this;
	}

	float dot(const Vector3d& other) const {
		DirectX::XMVECTOR result = DirectX::XMVector3Dot(internal(), other.internal());
		return DirectX::XMVectorGetX(result);
	}

	Vector3d cross(const Vector3d& other) const {
		return DirectX::XMVector3Cross(internal(), other.internal());
	}

	Proxy x() {
		return Proxy(&DirectX::XMVectorSetX, internal(), DirectX::XMVectorGetX(internal()));
	}

	float x() const {
		return DirectX::XMVectorGetX(internal());
	}

	Proxy y() {
		return Proxy(&DirectX::XMVectorSetY, internal(), DirectX::XMVectorGetY(internal()));
	}

	float y() const {
		return DirectX::XMVectorGetY(internal());
	}

	Proxy z() {
		return Proxy(&DirectX::XMVectorSetZ, internal(), DirectX::XMVectorGetZ(internal()));
	}

	float z() const {
		return DirectX::XMVectorGetZ(internal());
	}

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

	operator const DirectX::XMVECTOR&() {
		return internal();
	}

	Vector2d operator *(const Matrix& matrix) {
		return DirectX::XMVector2Transform(internal(), matrix.internal());
	}

	Vector2d& operator *=(const Matrix& matrix) {
		internal() = DirectX::XMVector2Transform(internal(), matrix.internal());
		return *this;
	}

	float dot(const Vector2d& other) const {
		DirectX::XMVECTOR result = DirectX::XMVector2Dot(internal(), other.internal());
		return DirectX::XMVectorGetX(result);
	}

	Proxy x() {
		return Proxy(&DirectX::XMVectorSetX, internal(), DirectX::XMVectorGetX(internal()));
	}

	float x() const {
		return DirectX::XMVectorGetX(internal());
	}

	Proxy y() {
		return Proxy(&DirectX::XMVectorSetY, internal(), DirectX::XMVectorGetY(internal()));
	}

	float y() const {
		return DirectX::XMVectorGetY(internal());
	}

};

}
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_MATH_VECTOR_HPP_ */
