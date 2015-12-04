#include "ShaderResource.hpp"

#include <stdexcept>

#include <d3d11.h>

#include "Device.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::graphics;

void ShaderResource::bind(Device& device, ShaderType shaderType, size_t slot) {
	static ID3D11SamplerState* ss = nullptr;

	if (!ss) {
		D3D11_SAMPLER_DESC desc;
		std::memset(&desc, 0, sizeof(desc));

		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.Filter = D3D11_FILTER_ANISOTROPIC;
		desc.MaxAnisotropy = 4;

		device.d3dDevice()->CreateSamplerState(&desc, &ss);
	}

	device.d3dDeviceContext()->PSSetSamplers(0, 1, &ss);

	switch (shaderType) {
	case ShaderType::VERTEX:
		device.d3dDeviceContext()->VSSetShaderResources(static_cast<UINT>(slot), 1, &shaderResourceView_.get());
		break;
	case ShaderType::PIXEL:
		device.d3dDeviceContext()->PSSetShaderResources(static_cast<UINT>(slot), 1, &shaderResourceView_.get());
		break;
	default:
		throw std::runtime_error("Unknown shader type"); // TODO: custom exception type
	}
}
