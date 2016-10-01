#ifndef _COCONUT_PULP_RENDERER_SHADER_CALLBACKPARAMETER_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_CALLBACKPARAMETER_HPP_

#include <functional>

#include "Parameter.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace shader {

template <class InputType, class OutputType>
class CallbackParameter final : public ConcreteParameter<InputType, OutputType> {
public:
	
	using Callback = std::function<void (OutputType&, const InputType&)>;

	CallbackParameter(Callback callback) :
		callback_(callback)
	{
	}

protected:

	void updateThis(OutputType& output, const InputType& input) const override {
		callback_(output, input);
	}

private:

	Callback callback_;

};

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_CALLBACKPARAMETER_HPP_ */
