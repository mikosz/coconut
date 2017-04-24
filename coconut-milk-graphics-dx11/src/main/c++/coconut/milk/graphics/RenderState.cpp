#include "RenderState.hpp"

#include "DirectXError.hpp"
#include "Renderer.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::graphics;

namespace /* anonymous */ {

system::COMWrapper<ID3D11RasterizerState> createRasteriserState(
	Renderer& renderer,
	const RenderState::Configuration& configuration
	)
{
	auto rasteriserDesc = D3D11_RASTERIZER_DESC();
	std::memset(&rasteriserDesc, 0, sizeof(rasteriserDesc));

	rasteriserDesc.CullMode = static_cast<D3D11_CULL_MODE>(configuration.cullMode);
	rasteriserDesc.FillMode = static_cast<D3D11_FILL_MODE>(configuration.fillMode);
	rasteriserDesc.FrontCounterClockwise = configuration.frontCounterClockwise;

	auto rasteriserState = system::COMWrapper<ID3D11RasterizerState>();
	checkDirectXCall(
		renderer.internalDevice().CreateRasterizerState(&rasteriserDesc, &rasteriserState.get()),
		"Failed to create a rasteriser state"
		);

	return rasteriserState;
}

} // anonymous namespace

RenderState::RenderState(Renderer& renderer, const Configuration& configuration) :
	rasteriserState_(createRasteriserState(renderer, configuration))
{
}
