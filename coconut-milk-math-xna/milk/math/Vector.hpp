#ifndef _COCONUT_MILK_MATH_VECTOR_HPP_
#define _COCONUT_MILK_MATH_VECTOR_HPP_

#include <type_traits>
#include <iosfwd>
#include <cassert>

#include <boost/operators.hpp>

#include <DirectXMath.h>

namespace coconut {
namespace milk {
namespace math {

template <
	size_t DIMENSION_PARAM,
	typename std::enable_if<(DIMENSION_PARAM >= 1)>::type* = nullptr,
	typename std::enable_if<(DIMENSION_PARAM <= 4)>::type* = nullptr
	>
class Vector :
	boost::equality_comparable<Vector<DIMENSION_PARAM> >,
	boost::additive<Vector<DIMENSION_PARAM> >,
	boost::multiplicative<Vector<DIMENSION_PARAM>, float>
{
public:

	static const size_t DIMENSION = DIMENSION_PARAM;

	Vector() {
	}

	explicit Vector(DirectX::XMVECTOR xmvector) {
		store(xmvector);
	}

	template <size_t INDEX>
	float& get() {
		IndexChecker<(INDEX < DIMENSION)> checker;
		checker.checkVectorIndexInRange();
		return (&data_.x)[INDEX];
	}

	template <size_t INDEX>
	const float& get() const {
		IndexChecker<(INDEX < DIMENSION)> checker;
		checker.checkVectorIndexInRange();
		return (&data_.x)[INDEX];
	}

	float& get(size_t index) {
		assert(index < DIMENSION);

		if (index == 0) {
			return data_.x;
		} else if (index == 1) {
			return data_.y;
		} else if (index == 2) {
			return data_.z;
		} else if (index == 3) {
			return data_.w;
		}

		throw std::runtime_error("get() only accepts indices up to 3");
	}

	const float& get(size_t index) const {
		assert(index < DIMENSION);

		if (index == 0) {
			return data_.x;
		} else if (index == 1) {
			return data_.y;
		} else if (index == 2) {
			return data_.z;
		} else if (index == 3) {
			return data_.w;
		}

		throw std::runtime_error("get() only accepts indices up to 3");
	}

	float length() const {
		return DirectX::XMVectorGetX(DirectX::XMVector4Length(load()));
	}

	float near(const Vector& other, float epsilon) const {
		Vector difference = other - *this;
		for (size_t i = 0; i < Vector::DIMENSION; ++i) {
			if ((difference.get(i) > epsilon) || (difference.get(i) < -epsilon)) {
				return false;
			}
		}
		return true;
	}

	bool operator==(const Vector& other) const {
		for (size_t i = 0; i < Vector::DIMENSION; ++i) {
			if (get(i) != other.get(i)) {
				return false;
			}
		}

		return true;
	}

	Vector& operator*=(float scalar) {
		store(DirectX::XMVectorMultiply(load(), DirectX::XMVectorReplicate(scalar)));
		return *this;
	}

	Vector& operator/=(float scalar) {
		store(DirectX::XMVectorDivide(load(), DirectX::XMVectorReplicate(scalar)));
		return *this;
	}

	float dot(const Vector& other) const {
		return DirectX::XMVectorGetX(DirectX::XMVector4Dot(load(), other.load()));
	}

	Vector& operator+=(const Vector& other) {
		store(DirectX::XMVectorAdd(load(), other.load()));
		return *this;
	}

	Vector& operator-=(const Vector& other) {
		store(DirectX::XMVectorSubtract(load(), other.load()));
		return *this;
	}

	Vector operator-() const {
		return -1.0f * (*this);
	}

	DirectX::XMVECTOR toXMVECTOR() const {
		return load();
	}

	friend std::ostream& operator<<(std::ostream& os, const Vector& vector) {
		os << "<" << vector.get<0>();
		for (size_t i = 1; i < DIMENSION; ++i) {
			os << ", " << vector.get(i);
		}
		os << ">";

		return os;
	}

protected:

	Vector(float x, float y, float z, float w) :
		data_(x, y, z, w)
	{
	}

	DirectX::XMVECTOR load() const {
		return DirectX::XMLoadFloat4(&data_);
	}

	void store(const DirectX::XMVECTOR& vector) {
		DirectX::XMStoreFloat4(&data_, vector);
	}

private:

	template <bool valid>
	struct IndexChecker {
	};

	template <>
	struct IndexChecker<true> {

		// this function will be missing if get<i>() is called with too large "i"
		void checkVectorIndexInRange() {
		}

	};

	DirectX::XMFLOAT4 data_;

};

template <class VectorType>
float dot(const VectorType& lhs, const VectorType& rhs) {
	return lhs.dot(rhs);
}

template <class VectorType>
float length(const VectorType& vector) {
	return vector.length();
}

class Vector1d : public Vector<1> {
public:

	Vector1d() {
	}

	explicit Vector1d(float x) : Vector(x, 0.0f, 0.0f, 0.0f) {
	}

	explicit Vector1d(DirectX::XMVECTOR xmvector) : Vector(xmvector) {
	}

	float& x() {
		return get<0>();
	}

	const float& x() const {
		return get<0>();
	}

};

class Vector2d : public Vector<2> {
public:

	Vector2d() {
	}

	explicit Vector2d(float x) : Vector(x, 0.0f, 0.0f, 0.0f) {
	}

	Vector2d(float x, float y) : Vector(x, y, 0.0f, 0.0f) {
	}

	explicit Vector2d(DirectX::XMVECTOR xmvector) : Vector(xmvector) {
	}

	float& x() {
		return get<0>();
	}

	const float& x() const {
		return get<0>();
	}

	float& y() {
		return get<1>();
	}

	const float& y() const {
		return get<1>();
	}

};

class Vector3d : public Vector<3> {
public:

	Vector3d() {
	}

	explicit Vector3d(float x) : Vector(x, 0.0f, 0.0f, 0.0f) {
	}

	Vector3d(float x, float y) : Vector(x, y, 0.0f, 0.0f) {
	}

	Vector3d(float x, float y, float z) : Vector(x, y, z, 0.0f) {
	}

	explicit Vector3d(DirectX::XMVECTOR xmvector) : Vector(xmvector) {
	}

	Vector3d& crossEq(const Vector3d& rhs) {
		store(DirectX::XMVector3Cross(load(), rhs.load()));
		return *this;
	}

	Vector3d cross(const Vector3d& rhs) const {
		Vector3d result = *this;
		result.crossEq(rhs);
		return result;
	}

	float& x() {
		return get<0>();
	}

	const float& x() const {
		return get<0>();
	}

	float& y() {
		return get<1>();
	}

	const float& y() const {
		return get<1>();
	}

	float& z() {
		return get<2>();
	}

	const float& z() const {
		return get<2>();
	}

};

inline Vector3d cross(const Vector3d& lhs, const Vector3d& rhs) {
	return lhs.cross(rhs);
}

class Vector4d : public Vector<4> {
public:

	Vector4d() {
	}

	explicit Vector4d(float x) : Vector(x, 0.0f, 0.0f, 0.0f) {
	}

	Vector4d(float x, float y) : Vector(x, y, 0.0f, 0.0f) {
	}

	Vector4d(float x, float y, float z) : Vector(x, y, z, 0.0f) {
	}

	Vector4d(float x, float y, float z, float w) : Vector(x, y, z, w) {
	}

	explicit Vector4d(DirectX::XMVECTOR xmvector) : Vector(xmvector) {
	}

	float& x() {
		return get<0>();
	}

	const float& x() const {
		return get<0>();
	}

	float& y() {
		return get<1>();
	}

	const float& y() const {
		return get<1>();
	}

	float& z() {
		return get<2>();
	}

	const float& z() const {
		return get<2>();
	}

	float& w() {
		return get<3>();
	}

	const float& w() const {
		return get<3>();
	}

};

}
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_MATH_VECTOR_HPP_ */
