#ifndef _COCONUT_PULP_RENDERER_SHADER_PASSFACTORY_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_PASSFACTORY_HPP_

#include <string>
#include <unordered_map>

#include <coconut-tools/design-pattern/factory.hpp>

#include "coconut/milk/graphics/Renderer.hpp"

#include "Pass.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace shader {

coconut_tools::design_pattern::Factory<

class PassFactory {
public:

	using PassId = std::string;

	PassFactory();

	PassSharedPtr getPass(milk::graphics::Renderer& graphicsRenderer, const PassId& passId);

private:

	using PassCache = std::unordered_map<PassId, PassSharedPtr>;

	PassCache passCache_;

	PassSharedPtr createPass(milk::graphics::Renderer& renderer, const PassId& passId);

};

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_PASSFACTORY_HPP_ */
