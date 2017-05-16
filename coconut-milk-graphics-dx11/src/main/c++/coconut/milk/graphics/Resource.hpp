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

	ID3D11Resource* internalResource() const noexcept {
		return resource_.get();
	}

protected:

	Resource(system::COMWrapper<ID3D11Resource> resource) :
		resource_(std::move(resource))
	{
	}

private:

	system::COMWrapper<ID3D11Resource> resource_;

};

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_RESOURCE_HPP_ */
