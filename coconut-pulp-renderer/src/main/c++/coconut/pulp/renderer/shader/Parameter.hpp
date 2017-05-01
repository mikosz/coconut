#ifndef _COCONUT_PULP_RENDERER_SHADER_PARAMETER_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_PARAMETER_HPP_

#include "Property.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace shader {

class Parameter {
public:

	Parameter(
		PropertyDescriptor propertyDescriptor,
		Property::DataType dataType,
		size_t offset
		) :
		propertyDescriptor_(std::move(propertyDescriptor)),
		dataType_(dataType),
		offset_(offset)
	{
	}

private:

	PropertyDescriptor propertyDescriptor_;

	Property::DataType dataType_;

	size_t offset_;

};

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_PARAMETER_HPP_ */
