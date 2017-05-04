#ifndef _COCONUT_PULP_RENDERER_SHADER_RESOURCE_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_RESOURCE_HPP_

#include "coconut/milk/graphics/ShaderType.hpp"
#include "Property.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace shader {

class Resource {
public:

	Resource(
		PropertyDescriptor propertyDescriptor,
		milk::graphics::ShaderReflection::ResourceInfo::Type type,
		milk::graphics::ShaderType stage,
		size_t slot
		) noexcept :
		propertyDescriptor_(std::move(propertyDescriptor)),
		type_(type),
		stage_(stage),
		slot_(slot)
	{
	}

	void bind(DrawCommand& drawCommand, const Properties& properties) const {
		properties.bindResource(drawCommand, propertyDescriptor_, type_, stage_, slot_);
	}

private:

	PropertyDescriptor propertyDescriptor_;

	milk::graphics::ShaderType stage_;

	milk::graphics::ShaderReflection::ResourceInfo::Type type_;

	size_t slot_;

};

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_RESOURCE_HPP_ */
