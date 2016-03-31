#include "Sampler.hpp"

#include "DirectXError.hpp"
#include "Renderer.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::graphics;

Sampler::Sampler(Renderer& renderer, const Configuration& configuration) {
	D3D11_SAMPLER_DESC samplerDesc;
	std::memset(&samplerDesc, 0, sizeof(samplerDesc));

	samplerDesc.AddressU = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(configuration.addressModeU);
	samplerDesc.AddressV = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(configuration.addressModeV);
	samplerDesc.AddressW = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(configuration.addressModeW);

	samplerDesc.Filter = static_cast<D3D11_FILTER>(configuration.filter);

	checkDirectXCall(
		renderer.internalDevice().CreateSamplerState(samplerDesc, &samplerState_.get()),
		"Failed to create a sampler state"
		);
}
