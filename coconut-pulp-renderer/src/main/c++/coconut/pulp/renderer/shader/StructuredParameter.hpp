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

	using Subparameter = std::shared_ptr<Parameter>;

	void addSubparameter(Subparameter subparameter) {
		subparameters_.emplace_back(std::move(subparameter));
	}

protected:

	void updateThis(void* output, const void* input) const override {
		auto* const bufferStart = reinterpret_cast<std::uint8_t*>(output);
		auto* fieldIt = bufferStart;

		for (auto& subparameter : subparameters_) {
			subparameter->update(fieldIt, input);
			fieldIt += subparameter->size();
		}

		auto* const expected = bufferStart + size();
		if (fieldIt != expected) {
			std::ostringstream err;
			err << "Expected end pointer to be at " << expected << ", got " << *fieldIt;
#pragma message("!!! TODO: exception")
			throw coconut_tools::exceptions::LogicError(err.str());
		}
	}

	bool requires16ByteAlignment() const noexcept override {
		return true;
	}

private:

	using Subparameters = std::vector<Subparameter>;

	Subparameters subparameters_;

	using Parameter::setNext;

};

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_STRUCTUREDPARAMETER_HPP_ */
