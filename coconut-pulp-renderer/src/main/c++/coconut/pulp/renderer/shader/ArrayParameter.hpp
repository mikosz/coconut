#ifndef _COCONUT_PULP_RENDERER_SHADER_ARRAYPARAMETER_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_ARRAYPARAMETER_HPP_

#include <vector>
#include <memory>

#include <coconut-tools/exceptions/LogicError.hpp>

#include "Parameter.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace shader {

template <class... UpdateArguments>
class ArrayParameter : public Parameter<UpdateArguments...> {
public:

	using Element = Parameter<UpdateArguments..., size_t>;

	using ElementPtr = std::shared_ptr<Element>;

	ArrayParameter(
		ElementPtr element,
		size_t count
		) :
		Parameter(element->size() * count),
		element_(element),
		count_(count)
	{
	}

	void update(void* buffer, const UpdateArguments&... data) override {
		auto* fieldIt = reinterpret_cast<std::uint8_t*>(buffer);
		for (size_t i = 0; i < count_; ++i) {
			element_->update(fieldIt, data..., i);
			fieldIt += element_->size(); // TODO: padding here
		}

		auto* const expected = reinterpret_cast<std::uint8_t*>(buffer) + size();
		if (fieldIt != expected) {
			std::ostringstream err;
			err << "Expected end pointer to be at " << expected << ", got " << *fieldIt;
			throw coconut_tools::exceptions::LogicError(err.str());
		}
	}

private:

	ElementPtr element_;

	size_t count_;

};

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_ARRAYPARAMETER_HPP_ */
