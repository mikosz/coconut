#ifndef _COCONUT_MILK_GRAPHICS_RESOURCE_HPP_
#define _COCONUT_MILK_GRAPHICS_RESOURCE_HPP_

#include <d3d11.h>
#include "coconut/milk/system/cleanup-windows-macros.hpp"

#include "coconut/milk/system/COMWrapper.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class Resource {
public:

	Resource() = default;

	virtual ~Resource() = default;

	void reset() {
		resource_.reset();
	}

	system::COMWrapper<ID3D11Resource> internalResource() const { // TODO: move to detail interface
		return resource_;
	}

	system::COMWrapper<ID3D11ShaderResourceView> internalShaderResourceView() const {
		return shaderResourceView_;
	}

protected:

	// TODO: is this how it's supposed to be done?
	Resource(const Resource&) = default;

	Resource(Resource&&) = default;

	Resource& operator=(const Resource&) = default;

	Resource& operator=(Resource&&) = default;

	system::COMWrapper<ID3D11Resource> resource_;

	system::COMWrapper<ID3D11ShaderResourceView> shaderResourceView_;

};

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_RESOURCE_HPP_ */
