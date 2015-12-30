#ifndef _COCONUT_PULP_RENDERER_SHADER_STRUCTUREDPARAMETER_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_STRUCTUREDPARAMETER_HPP_

#include <sstream>
#include <algorithm>
#include <numeric>

#include <coconut-tools/exceptions/LogicError.hpp>

#include "Parameter.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace shader {

template <class... UpdateArguments>
class StructuredParameter : public Parameter<UpdateArguments...> {
public:

	using Subparameter = std::shared_ptr<Parameter<UpdateArguments...>>;

	using Subparameters = std::vector<Subparameter>;

	StructuredParameter(Subparameters subparameters) :
		Parameter<UpdateArguments...>(totalSize(subparameters)),
		subparameters_(std::move(subparameters))
	{
	}

	void update(void* buffer, const UpdateArguments&... data) override {
		auto* fieldIt = reinterpret_cast<std::uint8_t*>(buffer);
		for (auto& subparameter : subparameters_) {
			subparameter->update(fieldIt, data...);
			fieldIt += subparameter->size();
		}

		auto* const expected = reinterpret_cast<std::uint8_t*>(buffer) + size();
		if (fieldIt != expected) {
			std::ostringstream err;
			err << "Expected end pointer to be at " << expected << ", got " << *fieldIt;
			throw coconut_tools::exceptions::LogicError(err.str());
		}
	}

private:

	Subparameters subparameters_;

	static size_t totalSize(const Subparameters& subparameters) {
		size_t result = 0;
		for (auto subparameter : subparameters) {
			result += subparameter->size();
		}
		return result;
	}

};

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_STRUCTUREDPARAMETER_HPP_ */
