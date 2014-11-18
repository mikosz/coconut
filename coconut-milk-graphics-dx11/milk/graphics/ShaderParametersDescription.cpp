#include "ShaderParametersDescription.hpp"

#include "Device.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::graphics;

void ShaderParametersDescription::MatrixElement::update(const Renderable& renderable) {
	const milk::math::Matrix& matrix = retrievalCallback_(renderable);
	
}

void ShaderParametersDescription::bind(Device& device, const Renderable& renderable) {
	for (size_t i = 0; i < elements_.size(); ++i) {
		ID3D11Buffer* buffer = elements_[i]->data(renderable);
		switch (shaderType_) {
		case VERTEX_SHADER:
			checkDirectXCall(
				device.d3dDeviceContext()->VSSetConstantBuffers(i, 1, buffer),
				"Failed to set vertex shader constant buffer"
				);
			break;
		case PIXEL_SHADER:
			checkDirectXCall(
				device.d3dDeviceContext()->PSSetConstantBuffers(i, 1, buffer),
				"Failed to set pixel shader constnat buffer"
				);
			break;
		default:
			throw std::runtime_error("Unknown shader type");
		}
	}
}

void ShaderParametersDescription::push(std::shared_ptr<Element> element) {
	elements_.push_back(element);
}
