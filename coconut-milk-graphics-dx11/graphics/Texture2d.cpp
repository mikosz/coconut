#include "Texture2d.hpp"

#include <cstring>

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::graphics;

Texture2d::Texture2d(Device& device, const Configuration& configuration) {
	D3D11_TEXTURE2D_DESC desc;
	std::memset(&desc, 0, sizeof(desc));

	desc.Width = configuration.width;
	desc.Height = configuration.height;
	desc.MipLevels = configuration.mipLevels;
	desc.ArraySize = 1;
	desc.Format = static_cast<DXGI_FORMAT>(configuration.pixelFormat);
	desc.SampleDesc.Count = 1;
	desc.BindFlags = static_cast<UINT>(configuration.purpose);

	if (configuration.allowModifications) {
		if (configuration.allowCPURead) {
			desc.Usage = D3D11_USAGE_STAGING;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
		} else {
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}
	} else {
		if (configuration.allowCPURead) {
			desc.Usage = D3D11_USAGE_STAGING;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		} else {
			if (configuration.allowGPUWrite) {
				desc.Usage = D3D11_USAGE_DEFAULT;
			} else {
				desc.Usage = D3D11_USAGE_IMMUTABLE;
			}
		}
	}
}
