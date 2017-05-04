#ifndef _COCONUT_PULP_RENDERER_SHADER_PROPERTY_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_PROPERTY_HPP_

#include <vector>
#include <string>
#include <unordered_map>

#include <coconut-tools/exceptions/RuntimeError.hpp>
#include <coconut-tools/exceptions/LogicError.hpp>

#include "coconut/milk/graphics/ShaderReflection.hpp"
#include "coconut/milk/graphics/PixelFormat.hpp"
#include "coconut/pulp/primitive/Primitive.hpp"
#include "coconut/pulp/math/Vector.hpp"
#include "coconut/pulp/math/Matrix.hpp"
#include "PropertyId.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace shader {

class Property {
public:

	struct DataType {
		using Class = milk::graphics::ShaderReflection::Type::Class;
		using ScalarType = milk::graphics::ShaderReflection::Type::ScalarType;

		DataType(Class klass, ScalarType scalarType) :
			klass(klass),
			scalarType(scalarType)
		{
		}

		Class klass;
		ScalarType scalarType;
	};

	template <class T>
	Property(T model) :
		self_(std::make_unique<Model<T>>(std::move(model)))
	{
		// TODO: get rid of Primitive or stay with shared_ptrs, as it's not trivially_copyable
		// TOOD: OR figure out copying

		//static_assert(std::is_trivially_copyable_v<T>, "Type needs to be trivially copyable for use as property");
		//static_assert(sizeof(Model<T>) <= STORAGE_SIZE, "STORAGE_SIZE too small for this type");
		//static_assert(alignof(Model<T>) <= STORAGE_ALIGNMENT, "STORAGE_ALIGNMENT too small for this type");
		//new(&self_) Model<T>(std::move(model));
	}

	void* write(const PropertyId& id, void* buffer, DataType format) const {
		// return reinterpret_cast<const Concept*>(&self_)->write(id, buffer, format);
		return self_->write(id, buffer, format);
	}

private:

	class Concept {
	public:

		virtual ~Concept() = default;

		virtual void* write(const PropertyId& id, void* buffer, DataType format) const = 0;

	};

	template <class T>
	class Model : public Concept {
	public:

		// TODO: verify that construction / copying / descrucion of T is done correctly, as
		// I'm not sure.

		Model(T object) :
			object_(std::move(object))
		{
		}

		void* write(const PropertyId& id, void* buffer, DataType format) const override {
			return writeProperty(dereference(object_), id, buffer, format);
		}

	private:

		T object_;

		template <class V>
		static const V dereference(V object) {
			return std::move(object);
		}

		template <class V>
		static const V& dereference(const V* object) {
			return *object;
		}

	};

	std::shared_ptr<const Concept> self_;

	//static const auto STORAGE_SIZE = sizeof(Model<math::Matrix4x4>);

	//static const auto STORAGE_ALIGNMENT = alignof(Model<math::Matrix4x4>);

	//std::aligned_storage<STORAGE_SIZE, STORAGE_ALIGNMENT>::type self_;

};

class IncompatibleDataType : public coconut_tools::exceptions::RuntimeError {
public:

	IncompatibleDataType(std::string message) :
		coconut_tools::exceptions::RuntimeError(std::move(message))
	{
	}

};

class Properties {
public:

	const Property get(const std::string& id) const {
		return properties_.at(id); // TODO: use custom exception instead of at() everywhere here
	}

	void* write(const PropertyId& id, void* buffer, Property::DataType format) const {
		// TODO: replace at()
		return properties_.at(id.head().name).write(id, buffer, format);
	}

	void bind(std::string id, Property property) {
		auto result = properties_.emplace(std::move(id), std::move(property));
		if (!result.second) {
			throw coconut_tools::exceptions::LogicError("Property " + id + " is already bound");
		}
	}

private:

	std::unordered_map<std::string, Property> properties_;

};

inline void* writeProperty(
	const Properties& properties,
	const PropertyId& id,
	void* buffer,
	Property::DataType format
	)
{
	return properties.write(id.tail(), buffer, format);
}

template <class I>
inline std::enable_if_t<std::is_integral_v<I>, void*> writeProperty(
	I i,
	const PropertyId& id,
	void* buffer,
	Property::DataType format
	)
{
	if (format.klass != Property::DataType::Class::SCALAR) {
		throw IncompatibleDataType("Integers are not writeable to class " + toString(format.klass));
	}

	if (format.scalarType != Property::DataType::ScalarType::INT &&
		format.scalarType != Property::DataType::ScalarType::UINT)
	{
		throw IncompatibleDataType("Integers are not writeable to scalar type " + toString(format.scalarType));
	}

	// TODO: add assertion checking if cast doesn't result in a different value
	auto* target = reinterpret_cast<std::int32_t*>(buffer);
	// TODO: is this cast valid?
	*target = static_cast<std::int32_t>(i);
	return target + 1;
}

template <class F>
inline std::enable_if_t<std::is_floating_point_v<F>, void*> writeProperty(
	F f,
	const PropertyId& id,
	void* buffer,
	Property::DataType format
	)
{
	if (format.klass != Property::DataType::Class::SCALAR) {
		throw IncompatibleDataType("Floats are not writeable to class " + toString(format.klass));
	}

	if (format.scalarType != Property::DataType::ScalarType::FLOAT) {
		throw IncompatibleDataType("Floats are not writeable to scalar type " + toString(format.scalarType));
	}

	auto* target = reinterpret_cast<float*>(buffer);
	*target = static_cast<float>(f);
	return target + 1;
}

void* writeProperty(
	const math::Vec3& vec3,
	const PropertyId& id,
	void* buffer,
	Property::DataType format
	);

// TODO: merge for all vectors of floats
void* writeProperty(
	const math::Vec4& vec4,
	const PropertyId& id,
	void* buffer,
	Property::DataType format
	);

void* writeProperty(
	const math::Matrix4x4& matrix,
	const PropertyId& id,
	void* buffer,
	Property::DataType format
	);

inline void* writeProperty(
	const Primitive& primitive,
	const PropertyId& /* id */,
	void* buffer,
	Property::DataType format
	) {
#pragma message("REALLY TEMP")
	primitive.storeAs(buffer, milk::graphics::PixelFormat::R32G32B32_FLOAT); // TODO: TEMP TEMP TEEEEMP!
	return buffer; // TODO: temp! have primitive return end address
}

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_PROPERTY_HPP_ */
