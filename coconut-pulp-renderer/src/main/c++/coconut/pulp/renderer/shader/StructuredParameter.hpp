#ifndef _COCONUT_PULP_RENDERER_SHADER_STRUCTUREDPARAMETER_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_STRUCTUREDPARAMETER_HPP_

#include <sstream>
#include <algorithm>
#include <numeric>

#include <coconut-tools/exceptions/LogicError.hpp>

#include "coconut/milk/utils/bits.hpp"

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

	struct PaddedSubparameter {
	
		size_t padding;

		Subparameter subparameter;
	
		PaddedSubparameter(size_t padding, Subparameter subparameter) :
			padding(padding),
			subparameter(subparameter)
		{
		}

	};

	using PaddedSubparameters = std::vector<PaddedSubparameter>;

	StructuredParameter(PaddedSubparameters subparameters) :
		Parameter<UpdateArguments...>(totalSize(subparameters)),
		subparameters_(std::move(subparameters))
	{
	}

	void update(void* buffer, const UpdateArguments&... data) override {
		auto* const bufferStart = reinterpret_cast<std::uint8_t*>(buffer);
		auto* fieldIt = bufferStart;

		for (auto& subparameter : subparameters_) {
			fieldIt += subparameter.padding;
			subparameter.subparameter->update(fieldIt, data...);
			fieldIt += subparameter.subparameter->size();
		}

		auto* const expected = bufferStart + size();
		if (fieldIt != expected) {
			std::ostringstream err;
			err << "Expected end pointer to be at " << expected << ", got " << *fieldIt;
			throw coconut_tools::exceptions::LogicError(err.str());
		}
	}

	bool requires16ByteAlignment() const override {
		return true;
	}

private:

	PaddedSubparameters subparameters_;

	static size_t totalSize(const PaddedSubparameters& subparameters) {
		auto result = static_cast<size_t>(0);
		for (auto subparameter : subparameters) {
			result += subparameter.padding;
			result += subparameter.subparameter->size();
		}

		return result;
	}

};

template <class... UpdateArguments>
typename StructuredParameter<UpdateArguments...>::PaddedSubparameters layoutSubparameters(
	const typename StructuredParameter<UpdateArguments...>::Subparameters& subparameters
	) {
	typename StructuredParameter<UpdateArguments...>::PaddedSubparameters padded;
	padded.reserve(subparameters.size());

	auto offset = static_cast<size_t>(0);
	for (auto& subparameter : subparameters) {
		const auto subparameterSize = subparameter->size();
		const auto needsAlignment =
			offset % 16 != 0 &&
				(subparameter->requires16ByteAlignment() ||
				((offset / 16) != ((offset + subparameterSize - 1) / 16)))
			;
		auto padding = static_cast<size_t>(0);
		if (needsAlignment) {
			padding = 16 - (offset % 16);
		}

		padded.emplace_back(padding, subparameter);

		offset += padding + subparameterSize;
	}

	return padded;
}


} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_STRUCTUREDPARAMETER_HPP_ */
