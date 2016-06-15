#ifndef _COCONUT_PULP_RENDERER_PASSCONTEXT_HPP_
#define _COCONUT_PULP_RENDERER_PASSCONTEXT_HPP_

#include <unordered_map>

#include <coconut-tools/exceptions/LogicError.hpp>

#include "coconut/milk/graphics/Texture2d.hpp"
#include "coconut/milk/graphics/Viewport.hpp"

#include "shader/Pass.hpp"
#include "Material.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class Scene;
class Actor;
class Model;

// TODO: I don't like these pointers, move fields to private, stop passing this as value
class PassContext {
public:

	milk::graphics::Viewport* viewport = nullptr;

	milk::graphics::Texture2d* backBuffer = nullptr;

	milk::graphics::Texture2d* screenDepthStencil = nullptr;

	const Scene* scene = nullptr;

	const Actor* actor = nullptr;

	const Model* model = nullptr;

	const Material* material = nullptr;

	void reset() {
		viewport = nullptr;
		backBuffer = nullptr;
		screenDepthStencil = nullptr;
		scene = nullptr;
		actor = nullptr;
		model = nullptr;
		material = nullptr;
		passByType_.clear();
	}

	void setPass(ShaderPassType passType, shader::Pass* pass) {
		if (passByType_.count(passType) != 0) {
			throw coconut_tools::exceptions::LogicError("Shader pass already registered for pass type " + toString(passType));
		} else {
			passByType_.insert(std::make_pair(passType, pass));
		}
	}

	shader::Pass* getPass(ShaderPassType passType) {
		auto passIt = passByType_.find(passType);
		if (passIt == passByType_.end()) {
			return nullptr;
		} else {
			return passIt->second;
		}
	}

private:

	using PassByType = std::unordered_map<ShaderPassType, shader::Pass*>; // TODO: figure a better key type to avoid unnecessary duplication

	PassByType passByType_;

};

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_PASSCONTEXT_HPP_ */
