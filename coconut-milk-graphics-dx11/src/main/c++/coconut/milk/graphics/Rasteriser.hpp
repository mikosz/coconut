#ifndef _COCONUT_MILK_GRAPHICS_DX11_RASTERISER_HPP_
#define _COCONUT_MILK_GRAPHICS_DX11_RASTERISER_HPP_

#include <d3d11.h>
#include "coconut/milk/system/cleanup-windows-macros.hpp"

#include "coconut/milk/system/COMWrapper.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class Renderer;

class Rasteriser {
public:

	enum class CullMode {
		BACK = D3D11_CULL_BACK,
		FRONT = D3D11_CULL_FRONT,
		NONE = D3D11_CULL_NONE,
	};

	enum class FillMode {
		SOLID = D3D11_FILL_SOLID,
		WIREFRAME = D3D11_FILL_WIREFRAME,
	};

	struct Configuration {

		CullMode cullMode;

		FillMode fillMode;

	};

	Rasteriser(Renderer& renderer, const Configuration& configuration);

	ID3D11RasterizerState& internalRasteriserState() {
		return *rasteriserState_;
	}

private:

	system::COMWrapper<ID3D11RasterizerState> rasteriserState_;

};

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_DX11_RASTERISER_HPP_ */
