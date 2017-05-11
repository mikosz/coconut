#include "Input.hpp"

#include <algorithm>

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;
using namespace coconut::pulp::renderer::shader;

namespace /* anonymous */ {

size_t elementSize(const Input::Parameters& parameters) noexcept {
	if (parameters.empty()) {
		return 0u;
	} else {
		const auto& last = parameters.back();

		// TODO: TEMP!!! ++
		const auto size = last.dataType().columns * sizeof(float);
		// TODO: TEMP!!! --

		return size + last.offset();
	}
}

void* writeElement(void* buffer, const Properties& properties, const Input::Parameters& parameters) {
	auto start = reinterpret_cast<std::uint8_t*>(buffer);
	auto end = start;
	for (const auto& parameter : parameters) {
		assert(end <= start + parameter.offset());
		end = reinterpret_cast<std::uint8_t*>(parameter.write(start, properties));
	}
	return end;
}

} // anonymous namespace

Input::Input(
	milk::graphics::InputLayout layout,
	Parameters perVertexParameters,
	Parameters perInstanceParameters
	) noexcept :
	layout_(std::move(layout)),
	perVertexParameters_(std::move(perVertexParameters)),
	perInstanceParameters_(std::move(perInstanceParameters))
{
	const auto parameterComp = [](const auto& lhs, const auto& rhs) {
			return lhs.offset() < rhs.offset();
		};

	std::sort(perVertexParameters_.begin(), perVertexParameters_.end(), parameterComp);
	std::sort(perInstanceParameters_.begin(), perInstanceParameters_.end(), parameterComp);
}

size_t Input::vertexSize() const noexcept {
	return elementSize(perVertexParameters_);
}

void* Input::writeVertex(void* buffer, const Properties& properties) const {
	return writeElement(buffer, properties, perVertexParameters_);
}

size_t Input::instanceSize() const noexcept {
	return elementSize(perVertexParameters_);
}

void* Input::writeInstance(void* buffer, const Properties& properties) const {
	return writeElement(buffer, properties, perInstanceParameters_);
}
