#ifndef _COCONUT_MILK_GRAPHICS_DX11_TEXTURE_HPP_
#define _COCONUT_MILK_GRAPHICS_DX11_TEXTURE_HPP_

#include <type_traits>

#include <d3d11.h>
#include "coconut/milk/system/cleanup-windows-macros.hpp"

#include <coconut-tools/enums/Mask.hpp>

#include "coconut/milk/system/COMWrapper.hpp"

#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

#include "PixelFormat.hpp"
#include "Resource.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class Renderer;

class Texture : public Resource {
public:

	CT_MEMBER_FLAG_VALUES(
		CreationPurpose,
		(SHADER_RESOURCE)(D3D11_BIND_SHADER_RESOURCE)
		(RENDER_TARGET)(D3D11_BIND_RENDER_TARGET)
		(DEPTH_STENCIL)(D3D11_BIND_DEPTH_STENCIL)
		);

	void initialise(Renderer& renderer, coconut_tools::Mask<CreationPurpose> purposeFlags);

	void reset();

	ID3D11RenderTargetView& internalRenderTargetView() {
		return *renderTargetView_;
	}

	ID3D11DepthStencilView& internalDepthStencilView() {
		return *depthStencilView_;
	}

private:

	system::COMWrapper<ID3D11RenderTargetView> renderTargetView_;

	system::COMWrapper<ID3D11DepthStencilView> depthStencilView_;

};

CT_MAKE_POINTER_DEFINITIONS(Texture);

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_DX11_TEXTURE_HPP_ */
