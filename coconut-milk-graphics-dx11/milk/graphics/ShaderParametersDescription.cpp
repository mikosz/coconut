#include "ShaderParametersDescription.hpp"

#include <algorithm>
#include <functional>

#include "Device.hpp"
#include "milk/utils/RAIIHelper.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::graphics;

ShaderParametersDescription::Element::Element(Device& device, size_t size) :
	buffer_(device, Buffer::Configuration(size, size, true, false, false, Buffer::CONSTANT_BUFFER))
{
	assert(size % 16 == 0);
}

void ShaderParametersDescription::Element::update(Device& device, const Renderable& renderable) {
	auto data = buffer_.lock(device, Buffer::LockPurpose::WRITE_DISCARD);
	// TODO: is this replaceable with a lambda?
	utils::RAIIHelper unlockGuard(std::bind(&Buffer::unlock, &buffer_, std::ref(device)));

	doUpdate(data, renderable);
}

void ShaderParametersDescription::MatrixElement::doUpdate(void* buffer, const Renderable& renderable) {
	math::Matrix& matrix = *reinterpret_cast<math::Matrix*>(buffer);
	
	matrix = retrievalCallback_(renderable);
}

void ShaderParametersDescription::update(Device& device, const Renderable& renderable) {
	std::for_each(
		elements_.begin(),
		elements_.end(),
		[&device, &renderable](std::shared_ptr<Element> element) { element->update(device, renderable); }
	);
}

void ShaderParametersDescription::bind(Device& device, const Renderable& renderable) {
	for (size_t i = 0; i < elements_.size(); ++i) {
		ID3D11Buffer* buffer = elements_[i]->buffer(renderable);
		switch (shaderType_) {
		case VERTEX_SHADER:
			device.d3dDeviceContext()->VSSetConstantBuffers(i, 1, &buffer);
			break;
		case PIXEL_SHADER:
			device.d3dDeviceContext()->PSSetConstantBuffers(i, 1, &buffer);
			break;
		default:
			throw std::runtime_error("Unknown shader type");
		}
	}
}

void ShaderParametersDescription::push(std::shared_ptr<Element> element) {
	elements_.push_back(element);
}
