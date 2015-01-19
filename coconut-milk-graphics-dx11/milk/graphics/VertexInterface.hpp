#ifndef _COCONUT_MILK_GRAPHICS_VERTEXINTERFACE_HPP_
#define _COCONUT_MILK_GRAPHICS_VERTEXINTERFACE_HPP_

#include <stdexcept>

#include "milk/math/Vector.hpp"

#include "milk/utils/MakePointerDefinitionsMacro.hpp"

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

	virtual math::Vector4d ambientColour() const {
		throw std::runtime_error("ambientColour() not implemented");
	}

	virtual math::Vector4d diffuseColour() const {
		throw std::runtime_error("diffuseColour() not implemented");
	}

	virtual math::Vector4d specularColour() const {
		throw std::runtime_error("specularColour() not implemented");
	}

	virtual float specularExponent() const {
		throw std::runtime_error("specularExponent() not implemented");
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
