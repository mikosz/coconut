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

class Resource;
class Renderer;
class ConstantBuffer;
class IndexBuffer;
class VertexBuffer;
class Texture;
class Texture2d;
class Sampler;
class RenderState;
class InputLayout;
class Viewport;
enum class PixelFormat;

class CommandList {
public:

	using LockedData = std::unique_ptr<std::uint8_t, std::function<void(std::uint8_t*)>>;

	enum class LockPurpose {
		WRITE_DISCARD = D3D11_MAP_WRITE_DISCARD,
		WRITE_NO_OVERWRITE = D3D11_MAP_WRITE_NO_OVERWRITE,
	};

	CommandList();

	CommandList(system::COMWrapper<ID3D11DeviceContext> internalDeviceContext);

	void initialise(system::COMWrapper<ID3D11DeviceContext> internalDeviceContext);

	void drawIndexed(size_t startingIndex, size_t indexCount, PrimitiveTopology primitiveTopology);

	void drawIndexedInstanced(size_t vertexCountPerInstance, size_t instanceCount, size_t startingIndex,
		PrimitiveTopology primitiveTopology);

	LockedData lock(Resource& data, LockPurpose lockPurpose);

	void setRenderTarget(Texture2d& renderTarget, Texture2d& depthStencil);

	void setViewport(Viewport& viewport);

	void setInputLayout(InputLayout& inputLayout);

	void setVertexShader(VertexShader& vertexShader);

	void setPixelShader(PixelShader& pixelShader);

	void setConstantBuffer(ConstantBuffer& buffer, ShaderType stage, size_t slot);

	void setIndexBuffer(IndexBuffer& buffer, size_t offset);

	void setVertexBuffer(VertexBuffer& buffer, size_t slot);

	void setInstanceDataBuffer(VertexBuffer& buffer, size_t slot);

	void setTexture(const Texture& texture, ShaderType stage, size_t slot);

	void setSampler(Sampler& sampler, ShaderType stage, size_t slot);

	void setRenderState(const RenderState& renderState);

	ID3D11DeviceContext& internalDeviceContext() {
		return *deviceContext_;
	}

private:

	system::COMWrapper<ID3D11DeviceContext> deviceContext_;

};

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_DX11_COMMANDLIST_HPP_ */
