#ifndef _COCONUT_MILK_GRAPHICS_DATA_HPP_
#define _COCONUT_MILK_GRAPHICS_DATA_HPP_

#include <d3d11.h>
#include "coconut/milk/system/cleanup-windows-macros.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class Data {
public:

	Data() = default;

	virtual ~Data() = default;

	virtual ID3D11Resource& internalResource() = 0;

protected:

	// TODO: is this how it's supposed to be done?
	Data(const Data&) = default;

	Data(Data&&) = default;

	Data& operator=(const Data&) = default;

	Data& operator=(Data&&) = default;

};

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_DATA_HPP_ */
