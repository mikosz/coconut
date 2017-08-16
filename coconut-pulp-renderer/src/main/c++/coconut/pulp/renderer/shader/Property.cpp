#include "Property.hpp"

#include <cstring>

#include "coconut/milk/graphics/Renderer.hpp"
#include "coconut/pulp/renderer/DrawCommand.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;
using namespace coconut::pulp::renderer::shader;

void* shader::writeDataProperty(
	void* buffer,
	const math::Vec2& vec2,
	const PropertyId& /*id*/,
	const Property::DataType& format
	)
{
	static_assert(std::is_trivially_copyable_v<pulp::math::Vec2>, "Vec2 is not trivially copyable");

	if (format.klass != Property::DataType::Class::VECTOR) {
		throw IncompatibleDataType("Vec2s are not writeable to class " + toString(format.klass));
	}

	if (format.scalarType != Property::DataType::ScalarType::FLOAT) {
		throw IncompatibleDataType("Vec2s are not writeable to scalar type " + toString(format.scalarType));
	}

	// TODO: check if columns <= 2?
	// TODO: merge with Vec4

	const auto size = sizeof(float) * format.columns;
	std::memcpy(buffer, &vec2, size);
	return reinterpret_cast<std::uint8_t*>(buffer) + size;
}

void* shader::writeDataProperty(
	void* buffer,
	const math::Vec3& vec3,
	const PropertyId& /*id*/,
	const Property::DataType& format
	)
{
	static_assert(std::is_trivially_copyable_v<pulp::math::Vec3>, "Vec3 is not trivially copyable");

	if (format.klass != Property::DataType::Class::VECTOR) {
		throw IncompatibleDataType("Vec3s are not writeable to class " + toString(format.klass));
	}

	if (format.scalarType != Property::DataType::ScalarType::FLOAT) {
		throw IncompatibleDataType("Vec3s are not writeable to scalar type " + toString(format.scalarType));
	}

	// TODO: check if columns <= 3?
	// TODO: merge with Vec4

	const auto size = sizeof(float) * format.columns;
	std::memcpy(buffer, &vec3, size);
	return reinterpret_cast<std::uint8_t*>(buffer) + size;
}

void* shader::writeDataProperty(
	void* buffer,
	const math::Vec4& vec4,
	const PropertyId& /*id*/,
	const Property::DataType& format
	)
{
	static_assert(std::is_trivially_copyable_v<pulp::math::Vec4>, "Vec4 is not trivially copyable");

	if (format.klass != Property::DataType::Class::VECTOR) {
		throw IncompatibleDataType("Vec4s are not writeable to class " + toString(format.klass));
	}

	if (format.scalarType != Property::DataType::ScalarType::FLOAT) {
		throw IncompatibleDataType("Vec4s are not writeable to scalar type " + toString(format.scalarType));
	}

	const auto size = sizeof(float) * format.columns;
	std::memcpy(buffer, &vec4, size);
	return reinterpret_cast<std::uint8_t*>(buffer) + size;
}

void* shader::writeDataProperty(
	void* buffer,
	const math::Matrix4x4& matrix,
	const PropertyId& /*id*/, // TODO: is this ever used?
	const Property::DataType& format
	)
{
	static_assert(std::is_trivially_copyable_v<pulp::math::Matrix4x4>, "Matrix is not trivially copyable");

	if (format.scalarType != Property::DataType::ScalarType::FLOAT) {
		throw IncompatibleDataType("Matrices are not writeable to scalar type " + toString(format.scalarType));
	}

	auto needsTranspose = false;

	if (format.klass == Property::DataType::Class::MATRIX_COLUMN_MAJOR) {
		needsTranspose = math::Matrix4x4::IS_ROW_MAJOR;
	} else if (format.klass == Property::DataType::Class::MATRIX_ROW_MAJOR) {
		needsTranspose = math::Matrix4x4::IS_COLUMN_MAJOR;
	} else {
		throw IncompatibleDataType("Matrices are not writeable to class " + toString(format.klass));
	}

#pragma warning(suppress: 4127)
	if (math::Matrix4x4::VECTOR_IS_SINGLE_ROW_MATRIX != milk::graphics::Renderer::VECTOR_IS_SINGLE_ROW_MATRIX) {
		needsTranspose = !needsTranspose;
	}

	auto* target = reinterpret_cast<pulp::math::Matrix4x4*>(buffer);

	if (needsTranspose) {
		*target = transpose(matrix.view());
	} else {
		*target = matrix;
	}

	return target + 1;
}

void* shader::writeDataProperty(
	void* buffer,
	const Primitive& primitive,
	const PropertyId& /* id */,
	const Property::DataType& format
	)
{
	// TODO: temp, do something with Primitive

	if (format.klass != Property::DataType::Class::VECTOR) {
		throw IncompatibleDataType("Vec4s are not writeable to class " + toString(format.klass));
	}

	if (format.scalarType != Property::DataType::ScalarType::FLOAT) {
		throw IncompatibleDataType("Vec4s are not writeable to scalar type " + toString(format.scalarType));
	}

	switch (format.columns) {
	case 2:
		primitive.storeAs(buffer, milk::graphics::PixelFormat::R32G32_FLOAT);
		break;
	case 3:
		primitive.storeAs(buffer, milk::graphics::PixelFormat::R32G32B32_FLOAT);
		break;
	case 4:
		primitive.storeAs(buffer, milk::graphics::PixelFormat::R32G32B32A32_FLOAT);
		break;
	}

	return reinterpret_cast<std::uint8_t*>(buffer) + sizeof(float) * format.columns;
}

void shader::bindResourceProperty(
	DrawCommand& drawCommand,
	const milk::graphics::Texture1d& texture,
	const PropertyId& /*id*/,
	milk::graphics::ShaderReflection::ResourceInfo::Type /*type*/,
	milk::graphics::ShaderType stage,
	size_t slot
	)
{
	drawCommand.addTexture(texture, stage, slot);
}

void shader::bindResourceProperty(
	DrawCommand& drawCommand,
	const milk::graphics::Texture2d& texture,
	const PropertyId& /*id*/,
	milk::graphics::ShaderReflection::ResourceInfo::Type /*type*/,
	milk::graphics::ShaderType stage,
	size_t slot
	)
{
	drawCommand.addTexture(texture, stage, slot);
}

void shader::bindResourceProperty(
	DrawCommand& drawCommand,
	const milk::graphics::Sampler& sampler,
	const PropertyId& /*id*/,
	milk::graphics::ShaderReflection::ResourceInfo::Type /*type*/,
	milk::graphics::ShaderType stage,
	size_t slot
	)
{
	drawCommand.addSampler(sampler, stage, slot);
}
