#include "ObserverAwareParameterWriter.hpp"

#include <string>
#include <stdexcept>

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;
using namespace coconut::pulp::renderer::shader;

void* ObserverAwareParameterWriter::write(void* target, ParameterId parameterId) {
	milk::math::Matrix* matrix = reinterpret_cast<milk::math::Matrix*>(target);

	switch (parameterId) {
	case ParameterId::VIEW_MATRIX:
		*matrix = camera_->viewTransformation().transposed();
		break;
	case ParameterId::PROJECTION_MATRIX:
		*matrix = lens_->projectionTransformation().transposed();
		break;
	default:
		throw std::logic_error("Unexpected parameter id " + std::to_string(static_cast<int>(parameterId)));
	}

	return ++matrix;
}
