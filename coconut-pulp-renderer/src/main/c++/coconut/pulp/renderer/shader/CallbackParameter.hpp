#ifndef _COCONUT_PULP_RENDERER_SHADER_CALLBACKPARAMETER_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_CALLBACKPARAMETER_HPP_

#include <functional>

#include "Parameter.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace shader {

template <class StoredType, class... UpdateArguments>
class CallbackParameter : public Parameter<UpdateArguments...> {
public:
	
	using Callback = std::function<void (StoredType&, const UpdateArguments&...)>;

	CallbackParameter(Callback callback) :
		callback_(callback)
	{
	}

	OperandType inputType() const noexcept override {
		return DeducedOperandType<UpdateArguments...>::type;
	}

	OperandType outputType() const noexcept override {
		return DeducedOperandType<StoredType>::type;
	}

	size_t size() const noexcept override {
		return sizeof(StoredType);
	}

protected:

	void update(void* buffer, const UpdateArguments&... data) const override {
		callback_(*reinterpret_cast<StoredType*>(buffer), data...);
	}

private:

	Callback callback_;

};

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_CALLBACKPARAMETER_HPP_ */
