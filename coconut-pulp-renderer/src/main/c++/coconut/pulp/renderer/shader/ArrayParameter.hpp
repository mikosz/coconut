#ifndef _COCONUT_PULP_RENDERER_SHADER_ARRAYPARAMETER_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_ARRAYPARAMETER_HPP_

#include <vector>
#include <memory>
#include <functional>

#include <coconut-tools/exceptions/LogicError.hpp>

#include "coconut/milk/utils/bits.hpp"

#include "Parameter.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace shader {

template <class ElementInputType, class ElementOutputType>
class ArrayParameter final : public Parameter {
public:

	using Callback = std::function<void (ElementOutputType&, const ElementInputType&, size_t)>;

	ArrayParameter(Callback callback, size_t size) :
		callback_(callback),
		size_(size)
	{
	}

	OperandType inputType() const noexcept override {
		return typename DeducedOperandType<ElementInputType>::type;
	}



protected:

	virtual void updateThis(void* output, const void* input) const override {
		auto* outputPtr = reinterpret_cast<ElementOutputType*>(output);
		const auto& concreteInput = *reinterpret_cast<const ElementInputType*>(input);
#pragma message("!!! TODO: padding!")
		for (size_t i = 0; i < size_; ++i) {
			callback_(*outputPtr, concreteInput, i);
			++outputPtr;
		}
	}

	virtual OperandType thisOutputType() const noexcept override {
		return typename DeducedOperandType<ElementOutputType>::type;
	}

	virtual size_t thisSize() const noexcept override {
#pragma message("!!! TODO: padding!")
		return sizeof(ElementOutputType) * size_;
	}

private:

	Callback callback_;

	size_t size_;

};

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_ARRAYPARAMETER_HPP_ */
