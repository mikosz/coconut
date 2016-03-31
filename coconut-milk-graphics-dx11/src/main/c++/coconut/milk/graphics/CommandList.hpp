#ifndef _COCONUT_MILK_GRAPHICS_DX11_COMMANDLIST_HPP_
#define _COCONUT_MILK_GRAPHICS_DX11_COMMANDLIST_HPP_

#include <d3d11.h>
#include "coconut/milk/system/cleanup-windows-macros.hpp"

#include "coconut/milk/system/COMWrapper.hpp"

#include "PrimitiveTopology.hpp"
#include "Shader.hpp"
#include "ShaderType.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class Renderer;
class Buffer;
class Texture2d;

class CommandList {
public:

	using LockedData = std::unique_ptr<void, std::function<void(void*)>>;

	enum class LockPurpose {
		WRITE_DISCARD = D3D11_MAP_WRITE_DISCARD,
		WRITE_NO_OVERWRITE = D3D11_MAP_WRITE_NO_OVERWRITE,
	};

	CommandList(Renderer& renderer);

	void draw(size_t startingIndex, size_t vertexCount, PrimitiveTopology primitiveTopology);

	LockedData lock(Data& data, LockPurpose lockPurpose);

	void setRenderTarget(Texture2d& renderTarget, Texture2d& depthStencil);

	void setVertexShader(VertexShader& vertexShader);

	void setPixelShader(PixelShader& pixelShader);

	void setBuffer(Buffer& buffer, ShaderType stage, size_t slot);

	void setTexture(Texture2d& texture, ShaderType stage, size_t slot);

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
