#ifndef _COCONUT_PULP_RENDERER_MESH_RENDERSTATECONFIGURATION_HPP_
#define _COCONUT_PULP_RENDERER_MESH_RENDERSTATECONFIGURATION_HPP_

// TODO: make one include file
#include <coconut-tools/serialisation/Serialiser.hpp>
#include <coconut-tools/serialisation/Deserialiser.hpp>
#include <coconut-tools/serialisation/make-serialisable-macro.hpp>

#include "coconut/milk/graphics/Rasteriser.hpp"

namespace coconut {
namespace pulp {
namespace mesh {

class RenderStateConfiguration {
public:

	using RasteriserConfiguration = milk::graphics::Rasteriser::Configuration;

	RasteriserConfiguration& rasteriserConfiguration() {
		return rasteriserConfiguration_;
	}

	const RasteriserConfiguration& rasteriserConfiguration() const {
		return rasteriserConfiguration_;
	}

private:

	// TODO: add blend and depth stencil states

	RasteriserConfiguration rasteriserConfiguration_;

};

CCN_MAKE_SERIALISABLE(SerialiserType, serialiser, RenderStateConfiguration, renderStateConfiguration) {
	serialiser(SerialiserType::Label("rasteriserConfiguration"), mesh.rasteriserConfiguration());
}

} // namespace mesh
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_MESH_RENDERSTATECONFIGURATION_HPP_ */
