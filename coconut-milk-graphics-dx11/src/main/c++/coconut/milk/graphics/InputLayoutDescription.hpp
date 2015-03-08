#ifndef _COCONUT_MILK_GRAPHICS_INPUTLAYOUT_HPP_
#define _COCONUT_MILK_GRAPHICS_INPUTLAYOUT_HPP_

#include <d3d11.h>

#include "coconut/milk/system/COMWrapper.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class Device;

class InputLayoutDescription {
public:

	virtual ~InputLayoutDescription() {
	}

	virtual system::COMWrapper<ID3D11InputLayout> makeLayout(
		Device& device,
		void* shaderData,
		size_t shaderSize
		) = 0;

protected:

	InputLayoutDescription() {
	}

};

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_INPUTLAYOUT_HPP_ */
