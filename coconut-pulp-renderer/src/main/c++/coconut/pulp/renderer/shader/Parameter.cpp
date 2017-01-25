#include "Parameter.hpp"

#include <vector>
#include <cstdint>
#include <cassert>
#include <algorithm>

#include "coconut/milk/graphics/ShaderReflection.hpp"

#include "coconut/milk/utils/bits.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;
using namespace coconut::pulp::renderer::shader;

void Parameter::update(void* output, const void* input) const {
	auto* buffer = reinterpret_cast<std::uint8_t*>(output);
	buffer += offset_;

	for (size_t i = 0; i < std::max<size_t>(1, arrayElements_); ++i) {
		buffer += i * arrayElementOffset_;

		if (next_) {
			std::vector<std::uint8_t> thisOutputBuffer(thisSize()); // TODO: avoid this allocation somehow + it's too big?
			updateThis(thisOutputBuffer.data(), input, i);
			next_->update(buffer, thisOutputBuffer.data());
		} else {
			updateThis(buffer, input, i);
		}
	}
}

auto Parameter::outputType() const noexcept -> OperandType {
	if (next_) {
		return next_->outputType();
	} else {
		return thisOutputType();
	}
}

void Parameter::setNext(std::shared_ptr<Parameter> next) {
	if (next_) {
		next_->setNext(next);
	} else {
		if (next->inputType() != thisOutputType()) {
			throw IncompatibleParameters(*this, *next);
		}
		next_ = std::move(next);
	}
}

size_t Parameter::size() const noexcept {
	if (next_) {
		return next_->size();
	} else {
		return thisSize();
	}
}
