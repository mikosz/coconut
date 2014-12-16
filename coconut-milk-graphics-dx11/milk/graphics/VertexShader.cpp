#include "VertexShader.hpp"

#include <algorithm>
#include <stdexcept>

#include "Device.hpp"
#include "DirectXError.hpp"
#include "InputLayoutDescription.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::graphics;

VertexShader::VertexShader(
	Device& device,
	void* data,
	size_t size,
	InputLayoutDescription& inputLayout
	) :
	inputLayout_(inputLayout.makeLayout(device, data, size))
{
	checkDirectXCall(
		device.d3dDevice()->CreateVertexShader(data, size, 0, &shader_.get()),
		"Failed to create a vertex shader"
		);
}

void VertexShader::bind(Device& device) {
	device.d3dDeviceContext()->IASetInputLayout(inputLayout_);
	device.d3dDeviceContext()->VSSetShader(shader_, 0, 0);
}
