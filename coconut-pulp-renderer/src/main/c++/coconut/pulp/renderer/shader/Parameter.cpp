#include "Parameter.hpp"

#include <vector>
#include <cstdint>
#include <cassert>
#include <algorithm>

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;
using namespace coconut::pulp::renderer::shader;

void* Parameter::update(void* output, const void* input) const {
	for (size_t i = 0; i < std::max<size_t>(1, arrayElements_); ++i) {
		if (next_) {
			std::vector<std::uint8_t> thisOutputBuffer(thisSize()); // TODO: avoid this allocation somehow
			updateThis(thisOutputBuffer.data(), input, i);
			output = next_->update(output, thisOutputBuffer.data());
		} else {
			output = updateThis(output, input, i);
		}
	}

	return output;
}

auto Parameter::outputType() const noexcept -> OperandType {
	if (next_) {
		return next_->outputType();
	} else {
		return thisOutputType();
	}
}

size_t Parameter::size() const noexcept {
	if (next_) {
		return next_->size();
	} else {
		return thisSize();
	}
}

void Parameter::setNext(std::shared_ptr<Parameter> next) {
	assert(!next_);

	if (next->inputType() != thisOutputType()) {
		throw IncompatibleParameters(*this, *next);
	}

	next_ = std::move(next);
}
