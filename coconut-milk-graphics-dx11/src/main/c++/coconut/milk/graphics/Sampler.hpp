#ifndef _COCONUT_MILK_GRAPHICS_DX11_SAMPLER_HPP_
#define _COCONUT_MILK_GRAPHICS_DX11_SAMPLER_HPP_

#include <d3d11.h>
#include "coconut/milk/system/cleanup-windows-macros.hpp"

#include <coconut-tools/enum.hpp>

#include "coconut/milk/system/COMWrapper.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class Renderer;

CCN_ENUM_VALUES( // TODO: make CCN_ENUM embeddable inside a class, move this back into Sampler
	AddressMode,
	(CLAMP)(D3D11_TEXTURE_ADDRESS_CLAMP)
	(WRAP)(D3D11_TEXTURE_ADDRESS_WRAP)
	(MIRROR)(D3D11_TEXTURE_ADDRESS_MIRROR)
	(MIRROR_ONCE)(D3D11_TEXTURE_ADDRESS_MIRROR_ONCE)
	);

CCN_ENUM_VALUES(
	Filter,
	(MIN_MAG_MIP_LINEAR)(D3D11_FILTER_MIN_MAG_MIP_LINEAR)
	(ANISOTROPIC)(D3D11_FILTER_ANISOTROPIC)
	);

class Sampler {
public:

	struct Configuration {

		AddressMode addressModeU;

		AddressMode addressModeV;

		AddressMode addressModeW;

		Filter filter;

	};

	Sampler() {
	}

	Sampler(Renderer& renderer, const Configuration& configuration);

	ID3D11SamplerState& internalSamplerState() {
		return *samplerState_;
	}

private:

	system::COMWrapper<ID3D11SamplerState> samplerState_;

};

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_DX11_SAMPLER_HPP_ */
