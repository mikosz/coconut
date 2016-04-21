#ifndef _COCONUT_MILK_GRAPHICS_DX11_COMMANDLIST_HPP_
#define _COCONUT_MILK_GRAPHICS_DX11_COMMANDLIST_HPP_

#include <cstdint>
#include <memory>
#include <functional>

#include <d3d11.h>
#include "coconut/milk/system/cleanup-windows-macros.hpp"

#include "coconut/milk/system/COMWrapper.hpp"

#include "PrimitiveTopology.hpp"
#include "Shader.hpp"
#include "ShaderType.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class Data;
class Renderer;
class ConstantBuffer;
class IndexBuffer;
class VertexBuffer;
class Texture;
class Texture2d;
class Sampler;
class Rasteriser;
class InputLayout;
enum class PixelFormat;

class CommandList {
public:

	using LockedData = std::unique_ptr<std::uint8_t, std::function<void(std::uint8_t*)>>;

	enum class LockPurpose {
		WRITE_DISCARD = D3D11_MAP_WRITE_DISCARD,
		WRITE_NO_OVERWRITE = D3D11_MAP_WRITE_NO_OVERWRITE,
	};

	CommandList(Renderer& renderer);

	void draw(size_t startingIndex, size_t indexCount, PrimitiveTopology primitiveTopology);

	LockedData lock(Data& data, LockPurpose lockPurpose);

	void setRenderTarget(Texture2d& renderTarget, Texture2d& depthStencil);

	void setInputLayout(InputLayout& inputLayout);

	void setVertexShader(VertexShader& vertexShader);

	void setPixelShader(PixelShader& pixelShader);

	void setConstantBuffer(ConstantBuffer& buffer, ShaderType stage, size_t slot);

	void setIndexBuffer(IndexBuffer& buffer, size_t offset);

	void setVertexBuffer(VertexBuffer& buffer, size_t slot);

	void setTexture(Texture& texture, ShaderType stage, size_t slot);

	void setSampler(Sampler& sampler, ShaderType stage, size_t slot);

	void setRasteriser(Rasteriser& rasteriser);

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
