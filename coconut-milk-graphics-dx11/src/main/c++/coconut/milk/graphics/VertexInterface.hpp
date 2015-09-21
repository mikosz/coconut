#ifndef _COCONUT_MILK_GRAPHICS_VERTEXINTERFACE_HPP_
#define _COCONUT_MILK_GRAPHICS_VERTEXINTERFACE_HPP_

#include <stdexcept>

#include "coconut/milk/math/Vector.hpp"

#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class VertexInterface {
public:

	virtual math::Vector3d position() const {
		throw std::runtime_error("position() not implemented");
	}

	virtual math::Vector2d textureCoordinate() const {
		throw std::runtime_error("textureCoordinate() not implemented");
	}

	virtual std::string materialName() const {
		throw std::runtime_error("materialName() not implemented");
	}

protected:

	virtual ~VertexInterface() {
	}

};

MAKE_POINTER_DEFINITIONS(VertexInterface);

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_VERTEXINTERFACE_HPP_ */
