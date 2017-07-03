#ifndef _COCONUT_MILK_GRAPHICS_DX11_RENDERSTATE_HPP_
#define _COCONUT_MILK_GRAPHICS_DX11_RENDERSTATE_HPP_

#include <d3d11.h>
#include "coconut/milk/system/cleanup-windows-macros.hpp"

#include <coconut-tools/enums.hpp>
#include <coconut-tools/serialisation.hpp>

#include "coconut/milk/system/COMWrapper.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class Renderer;

class RenderState {
public:

	CT_MEMBER_ENUM_VALUES(
		CullMode,
		(BACK)(D3D11_CULL_BACK)
		(FRONT)(D3D11_CULL_FRONT)
		(NONE)(D3D11_CULL_NONE)
		);

	CT_MEMBER_ENUM_VALUES(
		FillMode,
		(SOLID)(D3D11_FILL_SOLID)
		(WIREFRAME)(D3D11_FILL_WIREFRAME)
		);

	struct Configuration {

		CullMode cullMode = CullMode::BACK;

		FillMode fillMode = FillMode::SOLID;

		bool frontCounterClockwise = false;

		bool blendingEnabled = false;

	};

	RenderState(Renderer& renderer, const Configuration& configuration);

	ID3D11RasterizerState& internalRasteriserState() const {
		return *rasteriserState_;
	}

	ID3D11BlendState& internalBlendState() const {
		return *blendState_;
	}

private:

	system::COMWrapper<ID3D11RasterizerState> rasteriserState_;

	system::COMWrapper<ID3D11BlendState> blendState_;

};

CT_MAKE_SERIALISABLE(SerialiserType, serialiser, RenderState::Configuration, renderStateConfiguration) {
	serialiser(SerialiserType::Label("cullMode"), renderStateConfiguration.cullMode);
	serialiser(SerialiserType::Label("fillMode"), renderStateConfiguration.fillMode);
	serialiser(SerialiserType::Label("frontCounterClockwise"), renderStateConfiguration.frontCounterClockwise);
}

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_DX11_RENDERSTATE_HPP_ */
