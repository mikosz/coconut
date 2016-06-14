#ifndef _COCONUT_PULP_RENDERER_SHADER_CALLBACKPARAMETER_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_CALLBACKPARAMETER_HPP_

#include <functional>

#include "Parameter.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace shader {

template <class StoredType, class... UpdateArguments>
class CallbackParameter : public Parameter<UpdateArguments...> {
public:
	
	using TextureCallback = std::function<void (StoredType&, const UpdateArguments&...)>;

	CallbackParameter(TextureCallback callback) :
		Parameter(sizeof(StoredType)),
		textureCallback_(callback)
	{
	}

protected:

	void update(void* buffer, const UpdateArguments&... data) override {
		textureCallback_(*reinterpret_cast<StoredType*>(buffer), data...);
	}

private:

	TextureCallback textureCallback_;

};

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_CALLBACKPARAMETER_HPP_ */
