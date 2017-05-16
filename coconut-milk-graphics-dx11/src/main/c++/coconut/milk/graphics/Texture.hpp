#ifndef _COCONUT_MILK_GRAPHICS_DX11_TEXTURE_HPP_
#define _COCONUT_MILK_GRAPHICS_DX11_TEXTURE_HPP_

#include <d3d11.h>
#include "coconut/milk/system/cleanup-windows-macros.hpp"

#include <coconut-tools/enums/flag.hpp>

#include "coconut/milk/system/COMWrapper.hpp"
#include "Resource.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class Texture : public Resource {
public:

	CT_MEMBER_FLAG_VALUES(
		CreationPurpose,
		(SHADER_RESOURCE)(D3D11_BIND_SHADER_RESOURCE)
		(RENDER_TARGET)(D3D11_BIND_RENDER_TARGET)
		(DEPTH_STENCIL)(D3D11_BIND_DEPTH_STENCIL)
		);

protected:

	Texture() = default;

	Texture(system::COMWrapper<ID3D11Resource> resource) :
		Resource(std::move(resource))
	{
	}

};

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_DX11_TEXTURE_HPP_ */
