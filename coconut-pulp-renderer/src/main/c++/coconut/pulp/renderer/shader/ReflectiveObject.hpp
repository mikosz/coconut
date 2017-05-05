#ifndef _COCONUT_PULP_RENDERER_SHADER_REFLECTIVEOBJECT_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_REFLECTIVEOBJECT_HPP_

#include <unordered_map>
#include <vector>
#include <functional>
#include <algorithm>

#include <coconut-tools/utils/Range.hpp>
#include <coconut-tools/exceptions/RuntimeError.hpp>

#include "Property.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace shader {

// This reflection mechanism could probably be easily extended to a generic one and moved to tools.

template <class T>
class ReflectiveInterfaceBase {
public:

	// If performance of field/method lookup is an issue at any point in the future, optimisation
	// on the map can be done, because after initial construction it will never be modified.
	using Field = Property;

	using Fields = std::unordered_map<std::string, Field>;

	using Method = std::function<Property(const T&)>;

	using Methods = std::unordered_map<std::string, Method>;

	ReflectiveInterfaceBase() = default;

	template <class CompatibleT>
	ReflectiveInterfaceBase(const ReflectiveInterfaceBase<CompatibleT>& other) :
		fields_(other.fields())
	{
		for (auto compatibleMethodEntry : other.methods()) {
			methods_.emplace(
				std::move(compatibleMethodEntry.first),
				std::move(compatibleMethodEntry.second)
			);
		}
	}

	// TODO: field should be discerned from static field - a field should be an address of
	// an object's member and resolved when used!
	template <class... Args>
	void emplaceField(std::string id, Args&&... args) {
		fields_.emplace(std::move(id), std::forward<Args>(args)...);
	}

	void emplaceMethod(std::string id, Method method) {
		methods_.emplace(std::move(id), std::move(method));
	}

	const Property get(const T& object, const std::string& id) const {
		if (methods_.count(id)) {
			return methods_.at(id)(object);
		} else {
			return fields_.at(id);
		}
	}

	const Fields& fields() const {
		return fields_;
	}

	const std::unordered_map<std::string, Method> methods() const {
		return methods_;
	}

private:

	Fields fields_;

	Methods methods_;

};

template <class T>
class ReflectiveInterface : public ReflectiveInterfaceBase<T> {
public:

	ReflectiveInterface(); // TODO: make singleton!

};

template <class T>
class ReflectiveObject {
public:

	ReflectiveObject(const T& object) :
		object_(object)
	{
	}

	const T& object() const {
		return object_;
	}

private:

	const T& object_;

};

template <class T>
ReflectiveObject<T> makeReflectiveObject(const T& object) {
	return ReflectiveObject<T>(object);
}

template <class T>
void* writeDataProperty(
	void* buffer,
	const ReflectiveObject<T>& object,
	const PropertyId& id,
	const Property::DataType& format
	)
{
	// TODO: replace at()
	const auto referencedId = id.tail();

	const auto& iface = ReflectiveInterface<T>(); // TODO: singleton
	const auto& referencedProperty = iface.get(object.object(), referencedId.head().name);
	return referencedProperty.writeData(buffer, referencedId, format);
}

template <class T>
void bindResourceProperty(
	DrawCommand& drawCommand,
	const ReflectiveObject<T>& object,
	const PropertyId& id,
	milk::graphics::ShaderReflection::ResourceInfo::Type type,
	milk::graphics::ShaderType stage,
	size_t slot
	)
{
	// TODO: duplicated with write data

	// TODO: replace at()
	const auto referencedId = id.tail();

	const auto& iface = ReflectiveInterface<T>(); // TODO: singleton
	const auto& referencedProperty = iface.get(object.object(), referencedId.head().name);
	return referencedProperty.bindResource(drawCommand, referencedId, type, stage, slot);
}

template <class T>
class ReflectiveInterface<std::vector<T>> {
};

template <class T>
void* writeDataProperty(
	void* buffer,
	const ReflectiveObject<std::vector<T>>& vectorObject,
	const PropertyId& id,
	const Property::DataType& format
	)
{
	if (id.head().arraySize == 0) {
		// TODO: use some ShaderDefinitionError exception throughought this code
		throw coconut_tools::exceptions::RuntimeError(id.head().name + " is not used as an array");
	}

	const auto& vector = vectorObject.object();

	auto* lastWriteEnd = buffer;
	for (const auto index :
		coconut_tools::range(size_t(0), std::min(id.head().arraySize, vector.size())))
	{
		// TODO: array element offset doesn't work - either shader reflection info is wrong, or we're
		// interpretting it in a wrong way
		auto* elementPtr = reinterpret_cast<std::uint8_t*>(buffer) + id.head().arrayElementOffset * index;
		lastWriteEnd = writeDataProperty(elementPtr, makeReflectiveObject(vector[index]), id, format);
	}

	return lastWriteEnd;
}

template <class T>
void bindResourceProperty(
	DrawCommand& drawCommand,
	const ReflectiveObject<std::vector<T>>& vector,
	const PropertyId& id,
	milk::graphics::ShaderReflection::ResourceInfo::Type type,
	milk::graphics::ShaderType stage,
	size_t slot
	)
{
	auto oss = std::ostringstream();
	oss
		<< id
		<< " of type "
		<< typeid(vector).name()
		<< " cannot be bound as a shader resource";
	throw coconut_tools::exceptions::RuntimeError(oss.str());
}

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_REFLECTIVEOBJECT_HPP_ */
