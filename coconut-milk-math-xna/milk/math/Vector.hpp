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
class Vector : public boost::equality_comparable<Vector<DIMENSION_PARAM>, Vector<DIMENSION_PARAM> > {
public:

	static const size_t DIMENSION = DIMENSION_PARAM;

	Vector() {
	}

	template <size_t INDEX>
	float& get() {
		IndexChecker<INDEX> checker;
		checker.check();
		return (&data_.x)[INDEX];
	}

	template <size_t INDEX>
	const float& get() const {
		IndexChecker<INDEX> checker;
		checker.check();
		return (&data_.x)[INDEX];
	}

	float& get(size_t index) {
		assert(index < DIMENSION);

		if (index == 0) {
			return get<0>();
		} else if (index == 1) {
			return get<1>();
		} else if (index == 2) {
			return get<2>();
		} else if (index == 3) {
			return get<3>();
		}

		throw std::runtime_error("get() only accepts indices up to 3");
	}

	const float& get(size_t index) const {
		assert(index < DIMENSION);

		if (index == 0) {
			return get<0>();
		} else if (index == 1) {
			return get<1>();
		} else if (index == 2) {
			return get<2>();
		} else if (index == 3) {
			return get<3>();
		}

		throw std::runtime_error("get() only accepts indices up to 3");
	}

	bool operator==(const Vector& other) const {
		for (size_t i = 0; i < DIMENSION; ++i) {
			if (get(i) != other.get(i)) {
				return false;
			}
		}

		return true;
	}

protected:

	Vector(float x, float y, float z, float w) {
		DirectX::XMVectorSet(x, y, z, w);
	}

private:

	template <size_t INDEX, typename std::enable_if<(INDEX < DIMENSION)>::type* = nullptr>
	struct IndexChecker {

		void check() {
		}

	};

	DirectX::XMFLOAT4 data_;

	DirectX::XMVECTOR load() const {
		return DirectX::XMLoadFloat4(&data_);
	}

	void store(const DirectX::XMVECTOR& vector) {
		DirectX::XMStoreFloat4(&data_, vector);
	}

};

class Vector1d : public Vector<1> {
public:

	Vector1d() {
	}

	Vector1d(float x) : Vector(x, 0.0f, 0.0f, 0.0f) {
	}

	float& x() {
		return get<0>();
	}

	const float& x() const {
		return get<0>();
	}

};

std::ostream& operator<<(std::ostream& os, const Vector1d& vector) {
	return os << "<" << vector.x() << ">";
}

class Vector2d : public Vector<2> {
public:

	Vector2d() {
	}

	Vector2d(float x) : Vector(x, 0.0f, 0.0f, 0.0f) {
	}

	Vector2d(float x, float y) : Vector(x, y, 0.0f, 0.0f) {
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

std::ostream& operator<<(std::ostream& os, const Vector2d& vector) {
	return os << "<" << vector.x() << ", " << vector.y() << ">";
}

class Vector3d : public Vector<3> {
public:

	Vector3d() {
	}

	Vector3d(float x) : Vector(x, 0.0f, 0.0f, 0.0f) {
	}

	Vector3d(float x, float y) : Vector(x, y, 0.0f, 0.0f) {
	}

	Vector3d(float x, float y, float z) : Vector(x, y, z, 0.0f) {
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

std::ostream& operator<<(std::ostream& os, const Vector3d& vector) {
	return os << "<" << vector.x() << ", " << vector.y() << ", " << vector.z() << ">";
}

class Vector4d : public Vector<4> {
public:

	Vector4d() {
	}

	Vector4d(float x) : Vector(x, 0.0f, 0.0f, 0.0f) {
	}

	Vector4d(float x, float y) : Vector(x, y, 0.0f, 0.0f) {
	}

	Vector4d(float x, float y, float z) : Vector(x, y, z, 0.0f) {
	}

	Vector4d(float x, float y, float z, float w) : Vector(x, y, z, w) {
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

std::ostream& operator<<(std::ostream& os, const Vector4d& vector) {
	return os << "<" << vector.x() << ", " << vector.y() << ", " << vector.z() << ", " << vector.w() << ">";
}

}
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_MATH_VECTOR_HPP_ */
