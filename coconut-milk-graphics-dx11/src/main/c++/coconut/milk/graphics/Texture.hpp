#ifndef _COCONUT_MILK_GRAPHICS_DX11_TEXTURE_HPP_
#define _COCONUT_MILK_GRAPHICS_DX11_TEXTURE_HPP_

#include <type_traits>

#include <d3d11.h>
#include "coconut/milk/system/cleanup-windows-macros.hpp"

#include "coconut/milk/system/COMWrapper.hpp"

#include "PixelFormat.hpp"
#include "Data.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class Renderer;

class Texture : public Data {
public:

	enum class CreationPurpose {
		SHADER_RESOURCE = D3D11_BIND_SHADER_RESOURCE,
		RENDER_TARGET = D3D11_BIND_RENDER_TARGET,
		DEPTH_STENCIL = D3D11_BIND_DEPTH_STENCIL,
	};

	using CreationPurposeFlag = std::underlying_type_t<CreationPurpose>;

	void initialise(Renderer& renderer, CreationPurposeFlag purposeFlags);

	void reset();

	ID3D11RenderTargetView& internalRenderTargetView() {
		return *renderTargetView_;
	}

	ID3D11DepthStencilView& internalDepthStencilView() {
		return *depthStencilView_;
	}

	ID3D11ShaderResourceView& internalShaderResourceView() {
		return *shaderResourceView_;
	}

private:

	system::COMWrapper<ID3D11RenderTargetView> renderTargetView_;

	system::COMWrapper<ID3D11DepthStencilView> depthStencilView_;

	system::COMWrapper<ID3D11ShaderResourceView> shaderResourceView_;

};

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_DX11_TEXTURE_HPP_ */
