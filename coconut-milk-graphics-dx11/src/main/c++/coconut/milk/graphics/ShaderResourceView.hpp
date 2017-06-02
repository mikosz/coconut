#ifndef _COCONUT_MILK_GRAPHICS_DX11_SHADERRESOURCEVIEW_HPP_
#define _COCONUT_MILK_GRAPHICS_DX11_SHADERRESOURCEVIEW_HPP_

#include <d3d11.h>
#include "coconut/milk/system/cleanup-windows-macros.hpp"

#include "coconut/milk/system/COMWrapper.hpp"
#include "PixelFormat.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class Renderer;
class Texture;
class Buffer;

class ShaderResourceView {
public:

	struct Configuration {

		PixelFormat pixelFormat;

		size_t mipLevels = 1u;

		size_t mostDetailedMip = 0u;

	};

	ShaderResourceView() = default;

	ShaderResourceView(Renderer& renderer, const Texture& texture);

	ShaderResourceView(Renderer& renderer, const Texture& texture, Configuration configuration);

	ShaderResourceView(Renderer& renderer, const Buffer& buffer, PixelFormat elementFormat);

	ID3D11ShaderResourceView* internal() const noexcept {
		return srv_.get();
	}

private:

	system::COMWrapper<ID3D11ShaderResourceView> srv_;

};

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_DX11_SHADERRESOURCEVIEW_HPP_ */
