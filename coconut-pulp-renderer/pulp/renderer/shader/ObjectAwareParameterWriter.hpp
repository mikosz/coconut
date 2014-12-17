#ifndef _COCONUT_PULP_RENDERER_SHADER_OBJECTAWARESHADER_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_OBJECTAWARESHADER_HPP_

#include "../Model.hpp"
#include "ParameterWriter.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace shader {

class ObjectAwareParameterWriter : public ParameterWriter {
public:

	ObjectAwareParameterWriter(ModelSharedPtr model) :
		model_(model)
	{
	}

	void* write(void* target, ParameterId parameterId) override;

private:

	ModelSharedPtr model_;

};

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_OBJECTAWARESHADER_HPP_ */
