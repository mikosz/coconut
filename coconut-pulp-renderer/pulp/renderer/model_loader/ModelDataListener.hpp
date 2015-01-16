#ifndef _COCONUT_PULP_RENDERER_MODEL_LOADER_MODEL_DATA_LISTENER_HPP_
#define _COCONUT_PULP_RENDERER_MODEL_LOADER_MODEL_DATA_LISTENER_HPP_

#include "milk/graphics/PrimitiveTopology.hpp"

#include "milk/math/Vector.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace model_loader {

class ModelDataListener {
public:

	virtual void setVertexPosition(const milk::math::Vector3d& position) = 0;

	virtual void setVertexTextureCoordinate(const milk::math::Vector2d& textureCoordinate) = 0;

	virtual void setVertexNormal(const milk::math::Vector3d& normal) = 0;

	virtual void setVertexNormalNeedsCalculation() = 0;

	virtual void endVertex() = 0;

	virtual void endFace() = 0;

	virtual void endSmoothingGroup(milk::graphics::PrimitiveTopology primitiveTopology) = 0;

	virtual void endObject() = 0;

	virtual void endModel() = 0;

protected:

	virtual ~ModelDataListener() {
	}

};

} // namespace model_loader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_MODEL_LOADER_MODEL_DATA_LISTENER_HPP_ */
