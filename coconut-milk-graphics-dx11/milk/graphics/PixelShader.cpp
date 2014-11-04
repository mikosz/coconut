#include "PixelShader.hpp"

#include <stdexcept>

#include "Device.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::graphics;

PixelShader::PixelShader(Device& device, void* data, size_t size) {
	if (FAILED(device.d3dDevice()->CreatePixelShader(data, size, 0, &shader_.get()))) {
		throw std::runtime_error("Failed to create a pixel shader");
	}
};

void PixelShader::bind(Device& device) {
	device.d3dDeviceContext()->PSSetShader(shader_, 0, 0);
}
