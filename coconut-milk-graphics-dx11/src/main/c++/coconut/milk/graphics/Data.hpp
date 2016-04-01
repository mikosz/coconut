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

	Data(const Data&) = delete;

	virtual ~Data() = default;

	void operator=(const Data&) = delete;

	virtual ID3D11Resource& internalResource() = 0;

};

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_DATA_HPP_ */
