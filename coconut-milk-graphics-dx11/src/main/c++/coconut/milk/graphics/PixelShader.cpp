#include "PixelShader.hpp"

#include "DirectXError.hpp"
#include "Device.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::graphics;

PixelShader::PixelShader(Device& device, void* data, size_t size) {
	checkDirectXCall(
		device.d3dDevice()->CreatePixelShader(data, size, 0, &shader_.get()),
		"Failed to create a pixel shader"
		);
};

void PixelShader::bind(Device& device) const {
	device.d3dDeviceContext()->PSSetShader(shader_, 0, 0);
}
