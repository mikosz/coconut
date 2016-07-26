#ifndef _COCONUT_MILK_GRAPHICS_DX11_VIEWPORT_HPP_
#define _COCONUT_MILK_GRAPHICS_DX11_VIEWPORT_HPP_

#include <d3d11.h>
#include "coconut/milk/system/cleanup-windows-macros.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class Viewport {
public:

	struct Configuration {

		float width;

		float height;

		float minDepth;

		float maxDepth;

		float topLeftX;

		float topLeftY;

	};

	Viewport(const Configuration& configuration);

	D3D11_VIEWPORT& internalViewport() {
		return viewport_;
	}

private:

	D3D11_VIEWPORT viewport_;

};

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_DX11_VIEWPORT_HPP_ */
