#ifndef _COCONUT_PULP_RENDERER_SHADER_REFLECTIVEOBJECT_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_REFLECTIVEOBJECT_HPP_

#include <unordered_map>
#include <functional>

#include "Property.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace shader {

// This reflection mechanism could probably be easily extended to a generic one and moved to tools.

template <class T>
class ReflectiveInterface;

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

	template <class... Args>
	void emplaceField(std::string id, Args&&... args) {
		fields_.emplace(std::move(id), std::forward<Args>(args)...);
	}

	void emplaceMethod(std::string id, Method method) {
		methods_.emplace(std::move(id), std::move(method));
	}

	Property get(const T& object, const std::string& id) const {
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
class ReflectiveObject {
public:

	ReflectiveObject(const T& object) :
		object_(object)
	{
	}

	Property get(const std::string& id) const {
		const auto& iface = ReflectiveInterface<T>(); // TODO: singleton
		return iface.get(object_, id);
	}

private:

	const T& object_;

};

template <class T>
ReflectiveObject<T> makeReflectiveObject(const T& object) {
	return ReflectiveObject<T>(object);
}

template <class T>
void* writeProperty(const ReflectiveObject<T> object, PropertyId id, void* buffer, Property::DataType format) {
	if (!id.hasObject()) {
		return object.get(id.member()).write(id, buffer, format);
	} else {
		const auto& objectId = id.object();
		// TODO: handle array!
		return object.get(std::get<std::string>(objectId)).write(id.child(), buffer, format);
	}
}

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_REFLECTIVEOBJECT_HPP_ */
