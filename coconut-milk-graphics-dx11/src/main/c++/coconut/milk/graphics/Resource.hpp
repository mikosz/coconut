#ifndef _COCONUT_MILK_GRAPHICS_RESOURCE_HPP_
#define _COCONUT_MILK_GRAPHICS_RESOURCE_HPP_

#include <d3d11.h>
#include "coconut/milk/system/cleanup-windows-macros.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class Resource {
public:

	Resource() = default;

	virtual ~Resource() = default;

	virtual ID3D11Resource& internalResource() = 0;

	virtual ID3D11ShaderResourceView& internalShaderResourceView() const = 0; // TODO: temp

protected:

	// TODO: is this how it's supposed to be done?
	Resource(const Resource&) = default;

	Resource(Resource&&) = default;

	Resource& operator=(const Resource&) = default;

	Resource& operator=(Resource&&) = default;

};

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_RESOURCE_HPP_ */
