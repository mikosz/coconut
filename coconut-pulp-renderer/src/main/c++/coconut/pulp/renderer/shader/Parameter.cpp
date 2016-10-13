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
	auto* buffer = reinterpret_cast<std::uint8_t*>(output);
	buffer += padding_;

	for (size_t i = 0; i < std::max<size_t>(1, arrayElements_); ++i) {
		if (next_) {
			std::vector<std::uint8_t> thisOutputBuffer(thisSize()); // TODO: avoid this allocation somehow
			updateThis(thisOutputBuffer.data(), input, i);
			buffer = reinterpret_cast<std::uint8_t*>(next_->update(buffer, thisOutputBuffer.data()));
		} else {
			buffer = reinterpret_cast<std::uint8_t*>(updateThis(buffer, input, i));
		}
	}

	return buffer;
}

auto Parameter::outputType() const noexcept -> OperandType {
	if (next_) {
		return next_->outputType();
	} else {
		return thisOutputType();
	}
}

void Parameter::setNext(std::shared_ptr<Parameter> next) {
	assert(!next_);

	if (next->inputType() != thisOutputType()) {
		throw IncompatibleParameters(*this, *next);
	}

	next_ = std::move(next);
}

size_t Parameter::size() const noexcept {
	if (next_) {
		return next_->size() + padding_;
	} else {
		return thisSize() + padding_;
	}
}
