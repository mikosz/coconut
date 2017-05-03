#include "Property.hpp"

#include <cstring>

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;
using namespace coconut::pulp::renderer::shader;

void* shader::writeProperty(
	const math::Vec3& vec3,
	const PropertyId& id,
	void* buffer,
	Property::DataType format
	)
{
	static_assert(std::is_trivially_copyable_v<pulp::math::Vec3>, "Vec3 is not trivially copyable");

	if (format.klass != Property::DataType::Class::VECTOR) {
		throw IncompatibleDataType("Vec3s are not writeable to class " + toString(format.klass));
	}

	if (format.scalarType != Property::DataType::ScalarType::FLOAT) {
		throw IncompatibleDataType("Vec3s are not writeable to scalar type " + toString(format.scalarType));
	}

	// TODO: check vector dimensions!

	std::memcpy(buffer, &vec3, sizeof(vec3));

	return reinterpret_cast<pulp::math::Vec3*>(buffer) + 1;
}

void* shader::writeProperty(
	const math::Vec4& vec4,
	const PropertyId& id,
	void* buffer,
	Property::DataType format
	)
{
	static_assert(std::is_trivially_copyable_v<pulp::math::Vec4>, "Vec4 is not trivially copyable");

	if (format.klass != Property::DataType::Class::VECTOR) {
		throw IncompatibleDataType("Vec4s are not writeable to class " + toString(format.klass));
	}

	if (format.scalarType != Property::DataType::ScalarType::FLOAT) {
		throw IncompatibleDataType("Vec4s are not writeable to scalar type " + toString(format.scalarType));
	}

	// TODO: check vector dimensions!

	std::memcpy(buffer, &vec4, sizeof(vec4));

	return reinterpret_cast<pulp::math::Vec4*>(buffer) + 1;
}

void* shader::writeProperty(
	const math::Matrix4x4& matrix,
	const PropertyId& id,
	void* buffer,
	Property::DataType format
	)
{
	static_assert(std::is_trivially_copyable_v<pulp::math::Matrix4x4>, "Vec3 is not trivially copyable");

	// TODO: handle row-major matrices
	if (format.klass != Property::DataType::Class::MATRIX_COLUMN_MAJOR) {
		throw IncompatibleDataType("Matrices are not writeable to class " + toString(format.klass));
	}

	if (format.scalarType != Property::DataType::ScalarType::FLOAT) {
		throw IncompatibleDataType("Matrices are not writeable to scalar type " + toString(format.scalarType));
	}

	auto* target = reinterpret_cast<pulp::math::Matrix4x4*>(buffer);
	// TODO: don't transpose is same majority
	*target = matrix.transpose();

	return target + 1;
}
