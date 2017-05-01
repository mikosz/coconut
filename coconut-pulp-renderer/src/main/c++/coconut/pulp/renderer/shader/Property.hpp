#ifndef _COCONUT_PULP_RENDERER_SHADER_PROPERTY_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_PROPERTY_HPP_

#include <vector>
#include <string>
#include <tuple>

#include <coconut-tools/exceptions/RuntimeError.hpp>

#include "coconut/milk/graphics/ShaderReflection.hpp"
#include "coconut/milk/graphics/PixelFormat.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace shader {

class PropertyDescriptor {
public:

	using Object = std::tuple<std::string, size_t>;

	using Objects = std::vector<Object>;

	PropertyDescriptor(Objects objects, std::string member) noexcept :
		objects_(std::move(objects)),
		member_(std::move(member))
	{
	}

	const Objects& objects() const noexcept {
		return objects_;
	}

	const std::string& member() const noexcept {
		return member_;
	}

private:

	Objects objects_;

	std::string member_;

};

class PropertyId {
public:

	PropertyId(const PropertyDescriptor& descriptor) noexcept :
		objectsIt_(descriptor.objects().begin()),
		objectsEnd_(descriptor.objects().end()),
		member_(descriptor.member())
	{
	}

	PropertyId& child() noexcept {
		++objectsIt_;
		return *this;
	}

	bool hasObject() const noexcept {
		return objectsIt_ != objectsEnd_;
	}

	const PropertyDescriptor::Object& object() const noexcept {
		return *objectsIt_;
	}

	const std::string& member() const noexcept {
		return member_;
	}

private:

	PropertyDescriptor::Objects::const_iterator objectsIt_;

	PropertyDescriptor::Objects::const_iterator objectsEnd_;

	const std::string& member_;

};

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
	Property(T model) {
		static_assert(sizeof(Model<T>) <= STORAGE_SIZE, "STORAGE_SIZE too small for this type");
		static_assert(alignof(Model<T>) <= STORAGE_ALIGNMENT, "STORAGE_ALIGNMENT too small for this type");
		new(&self_) Model<T>(std::move(model));
	}

	void* write(const PropertyId& key, void* address, DataType format) const {
		return reinterpret_cast<const Concept*>(&self_)->write(key, address, format);
	}

private:

	class Concept {
	public:

		virtual ~Concept() = default;

		virtual void* write(const PropertyId& key, void* address, DataType format) const = 0;

	};

	template <class T>
	class Model : public Concept {
	public:

		Model(T object) :
			object_(std::move(object))
		{
			static_assert(std::is_pointer_v<T> || std::is_arithmetic_v<T>,
				"Properties should only be used as pointers or arithmetic types");
		}

		void* write(const PropertyId& key, void* address, DataType format) const override {
			writeProperty(dereference(object_), key, address, format);
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

	static const auto STORAGE_SIZE = sizeof(Model<float>);

	static const auto STORAGE_ALIGNMENT = alignof(Model<float>);

	std::aligned_storage<STORAGE_SIZE, STORAGE_ALIGNMENT>::type self_;

};

class IncompatibleDataType : public coconut_tools::exceptions::RuntimeError {
public:

	IncompatibleDataType(std::string message) :
		coconut_tools::exceptions::RuntimeError(std::move(message))
	{
	}

};

template <class I, class = std::enable_if_t<std::is_integral_v<I>>>
inline void* writeProperty(I i, const PropertyId& id, void* address, Property::DataType format) {
	assert(!id.hasObject());

	if (format.klass != Property::DataType::Class::SCALAR) {
		throw IncompatibleDataType("Integers are not writeable to class " + toString(format.klass));
	}

	if (format.scalarType != Property::DataType::ScalarType::INT &&
		format.scalarType != Property::DataType::ScalarType::UINT)
	{
		throw IncompatibleDataType("Integers are not writeable to scalar type " + toString(format.scalarType));
	}

	// TODO: add assertion checking if cast doesn't result in a different value
	auto* buffer = reinterpret_cast<std::int32_t*>(address);
	// TODO: is this cast valid?
	*buffer = static_cast<std::int32_t>(i);
	return buffer + 1;
}

template <class F, class = std::enable_if_t<std::is_floating_point_v<F>>>
inline void* writeProperty(F f, const PropertyId& id, void* address, Property::DataType format) {
	assert(!id.hasObject());

	if (format.klass != Property::DataType::Class::SCALAR) {
		throw IncompatibleDataType("Floats are not writeable to class " + toString(format.klass));
	}

	if (format.scalarType != Property::DataType::ScalarType::FLOAT) {
		throw IncompatibleDataType("Floats are not writeable to scalar type " + toString(format.scalarType));
	}

	auto* buffer = reinterpret_cast<float*>(address);
	*buffer = static_cast<float>(i);
	return buffer + 1;
}

class Properties {
public:

	const Property get(const std::string& id) const {
		return properties_.at(id);
	}

	void write(PropertyId id, void* address, Property::DataType format) const {
		if (!id.hasObject()) {
			properties_.at(id.member()).write(id, address, format);
		}
		else {
			const auto& object = id.object();
			properties_.at(object).write(id.child(), address, format);
		}
	}

	template <class... Args>
	void emplace(std::string id, Args&&... args) {
		properties_.emplace(std::move(id), std::forward<Args>(args)...);
	}

private:

	std::unordered_map<std::string, Property> properties_;

};

template <class Sub>
class PropertyHolder {
public:

	using Method = std::function<Property(const Sub&)>;

	template <class... Args>
	static void emplaceProperty(std::string id, Args&&... args) {
		properties_.emplace(std::move(id), std::forward<Args>(args)...);
	}

	static void emplaceMethod(std::string id, Method method) {
		methods.emplace(std::move(id), std::move(method));
	}

	const PropertyHolder<Sub>& properties() const noexcept {
		return *this;
	}

	const Property get(const std::string& id) const {
		if (methods.count(id)) {
			const auto& sub = static_cast<const Sub&>(*this);
			return methods.at(id)(sub);
		}
		else {
			return properties_.get(id);
		}
	}

private:

	static Properties properties_;

	static std::unordered_map<std::string, Method> methods;

};

template <class Sub>
Properties PropertyHolder<Sub>::properties_;

template <class Sub>
std::unordered_map<std::string, typename PropertyHolder<Sub>::Method> PropertyHolder<Sub>::methods;

template <class T>
inline void writeProperty(const PropertyHolder<T>& holder, PropertyId id, void* address, DataType format) {
	if (!id.hasObject()) {
		holder.get(id.member()).write(id, address, format);
	} else {
		const auto& object = id.object();
		holder.get(object).write(id.child(), address, format);
	}
}

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_PROPERTY_HPP_ */
