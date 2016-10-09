#ifndef _COCONUT_PULP_RENDERER_SHADER_STRUCTUREDPARAMETER_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_STRUCTUREDPARAMETER_HPP_

#include <memory>
#include <vector>

#include <coconut-tools/exceptions/LogicError.hpp>

#include "Parameter.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace shader {

class StructuredParameter final : public Parameter {
public:

	using Callback = std::function<const void* (const void*, size_t)>;

	using Subparameter = std::shared_ptr<Parameter>;

	StructuredParameter(Callback callback, OperandType inputType, size_t arrayElements = 0) :
		Parameter(arrayElements),
		inputType_(inputType),
		callback_(callback)
	{
	}

	void addSubparameter(Subparameter subparameter) {
#pragma message("if this class stays - verify input of subparameter is OBJECT")
		subparameters_.emplace_back(std::move(subparameter));
	}

	OperandType inputType() const noexcept override {
		return inputType_;
	}

protected:

	void* updateThis(void* output, const void* input, size_t arrayIndex) const override {
		const void* object = callback_(input, arrayIndex);

		for (auto& subparameter : subparameters_) {
			output = subparameter->update(output, object);
		}

		return output;
	}

	bool requires16ByteAlignment() const noexcept override {
		return true;
	}

	OperandType thisOutputType() const noexcept override {
		return OperandType::OBJECT;
	}

	size_t thisSize() const noexcept {
		return sizeof(void*);
	}

private:

	using Subparameters = std::vector<Subparameter>;

	OperandType inputType_;

	Callback callback_;

	Subparameters subparameters_;

	using Parameter::setNext;

};

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_STRUCTUREDPARAMETER_HPP_ */
