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
	auto desc = D3D11_RASTERIZER_DESC();
	std::memset(&desc, 0, sizeof(desc));

	desc.CullMode = static_cast<D3D11_CULL_MODE>(configuration.cullMode);
	desc.FillMode = static_cast<D3D11_FILL_MODE>(configuration.fillMode);
	desc.FrontCounterClockwise = configuration.frontCounterClockwise;
	desc.DepthClipEnable = configuration.depthClipEnable;

	auto rasteriserState = system::COMWrapper<ID3D11RasterizerState>();
	checkDirectXCall(
		renderer.internalDevice().CreateRasterizerState(&desc, &rasteriserState.get()),
		"Failed to create a rasteriser state"
		);

	return rasteriserState;
}

system::COMWrapper<ID3D11DepthStencilState> createDepthStencilState(
	Renderer& renderer,
	const RenderState::Configuration& /*configuration*/
	)
{
	auto desc = D3D11_DEPTH_STENCIL_DESC();
	std::memset(&desc, 0, sizeof(desc));

	desc.DepthEnable = true;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	desc.DepthFunc = D3D11_COMPARISON_LESS;

	auto depthStencilState = system::COMWrapper<ID3D11DepthStencilState>();
	checkDirectXCall(
		renderer.internalDevice().CreateDepthStencilState(&desc, &depthStencilState.get()),
		"Failed to create a depth stencil state"
		);

	return depthStencilState;
}

} // anonymous namespace

RenderState::RenderState(Renderer& renderer, const Configuration& configuration) :
	rasteriserState_(createRasteriserState(renderer, configuration)),
	depthStencilState_(createDepthStencilState(renderer, configuration))
{
}
