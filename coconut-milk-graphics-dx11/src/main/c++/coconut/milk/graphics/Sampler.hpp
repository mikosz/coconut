#ifndef _COCONUT_MILK_GRAPHICS_DX11_SAMPLER_HPP_
#define _COCONUT_MILK_GRAPHICS_DX11_SAMPLER_HPP_

#include <d3d11.h>
#include "coconut/milk/system/cleanup-windows-macros.hpp"

#include <coconut-tools/enums.hpp>
#include <coconut-tools/serialisation.hpp>

#include "coconut/milk/system/COMWrapper.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class Renderer;

class Sampler {
public:

	CT_MEMBER_ENUM_VALUES(
		AddressMode,
		(CLAMP)(D3D11_TEXTURE_ADDRESS_CLAMP)
		(WRAP)(D3D11_TEXTURE_ADDRESS_WRAP)
		(MIRROR)(D3D11_TEXTURE_ADDRESS_MIRROR)
		(MIRROR_ONCE)(D3D11_TEXTURE_ADDRESS_MIRROR_ONCE)
		);

	CT_MEMBER_ENUM_VALUES(
		Filter,
		(MIN_MAG_MIP_POINT)(D3D11_FILTER_MIN_MAG_MIP_POINT)
		(MIN_MAG_MIP_LINEAR)(D3D11_FILTER_MIN_MAG_MIP_LINEAR)
		(MIN_MAG_LINEAR_MIP_POINT)(D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT)
		(ANISOTROPIC)(D3D11_FILTER_ANISOTROPIC)
		);

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

CT_MAKE_SERIALISABLE(SerialiserType, serialiser, Sampler::Configuration, samplerConfiguration) {
	serialiser(SerialiserType::Label("addressModeU"), samplerConfiguration.addressModeU);
	serialiser(SerialiserType::Label("addressModeV"), samplerConfiguration.addressModeV);
	serialiser(SerialiserType::Label("addressModeW"), samplerConfiguration.addressModeW);
	serialiser(SerialiserType::Label("filter"), samplerConfiguration.filter);
}

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_DX11_SAMPLER_HPP_ */
