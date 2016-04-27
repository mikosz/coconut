#include "Rasteriser.hpp"

#include "DirectXError.hpp"
#include "Renderer.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::graphics;

Rasteriser::Rasteriser(Renderer& renderer, const Configuration& configuration) {
	D3D11_RASTERIZER_DESC rasteriserDesc;
	std::memset(&rasteriserDesc, 0, sizeof(rasteriserDesc));

	rasteriserDesc.CullMode = static_cast<D3D11_CULL_MODE>(configuration.cullMode);
	rasteriserDesc.FillMode = static_cast<D3D11_FILL_MODE>(configuration.fillMode);
	rasteriserDesc.FrontCounterClockwise = configuration.frontCounterClockwise;

	checkDirectXCall(
		renderer.internalDevice().CreateRasterizerState(&rasteriserDesc, &rasteriserState_.get()),
		"Failed to create a rasteriser state"
		);
}
