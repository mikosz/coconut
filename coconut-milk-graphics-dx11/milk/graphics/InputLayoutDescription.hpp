#ifndef _COCONUT_MILK_GRAPHICS_INPUTLAYOUT_HPP_
#define _COCONUT_MILK_GRAPHICS_INPUTLAYOUT_HPP_

#include <d3d11.h>

#include "milk/system/COMWrapper.hpp"

#include "milk/utils/MakePointerDefinitionsMacro.hpp"

#include "VertexInterface.hpp"

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
		) const = 0;

	virtual size_t vertexSize() const = 0;

	virtual void makeVertex(const VertexInterface& vertex, void* buffer) const = 0;

protected:

	InputLayoutDescription() {
	}

};

MAKE_POINTER_DEFINITIONS(InputLayoutDescription);

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_INPUTLAYOUT_HPP_ */
