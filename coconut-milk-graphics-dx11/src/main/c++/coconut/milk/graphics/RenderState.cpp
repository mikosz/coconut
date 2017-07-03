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

system::COMWrapper<ID3D11BlendState> createBlendState(
	Renderer& renderer,
	const RenderState::Configuration& configuration
	)
{
	auto blendDesc = D3D11_BLEND_DESC();
	std::memset(&blendDesc, 0, sizeof(blendDesc));

	// TODO: read config!!!
	blendDesc.AlphaToCoverageEnable = true;
	blendDesc.RenderTarget[0].BlendEnable = configuration.blendingEnabled;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	auto blendState = system::COMWrapper<ID3D11BlendState>();
	checkDirectXCall(
		renderer.internalDevice().CreateBlendState(&blendDesc, &blendState.get()),
		"Failed to create a blend state"
		);

	return blendState;
}

} // anonymous namespace

RenderState::RenderState(Renderer& renderer, const Configuration& configuration) :
	rasteriserState_(createRasteriserState(renderer, configuration)),
	blendState_(createBlendState(renderer, configuration))
{
}
