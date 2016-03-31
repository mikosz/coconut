#ifndef _COCONUT_MILK_GRAPHICS_DX11_COMMANDLIST_HPP_
#define _COCONUT_MILK_GRAPHICS_DX11_COMMANDLIST_HPP_

#include <d3d11.h>
#include "coconut/milk/system/cleanup-windows-macros.hpp"

#include "coconut/milk/system/COMWrapper.hpp"

#include "PrimitiveTopology.hpp"
#include "Texture2d.hpp"
#include "Shader.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class Renderer;

class CommandList {
public:

	CommandList(Renderer& renderer);

	void draw(size_t startingIndex, size_t vertexCount, PrimitiveTopology primitiveTopology);

	void setRenderTarget(Texture2d& renderTarget, Texture2d& depthStencil);

	void setVertexShader(VertexShader& vertexShader);

	void setPixelShader(PixelShader& pixelShader);

	ID3D11DeviceContext& internalDeviceContext() {
		return *d3dDeviceContext_;
	}

private:

	system::COMWrapper<ID3D11DeviceContext> d3dDeviceContext_;

};

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_DX11_COMMANDLIST_HPP_ */
