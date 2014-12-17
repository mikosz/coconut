#ifndef _COCONUT_PULP_RENDERER_SHADER_PARAMETERWRITERLIST_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_PARAMETERWRITERLIST_HPP_

#include <unordered_map>

#include "ParameterId.hpp"
#include "ParameterWriter.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace shader {

class ParameterWriterList : public ParameterWriter {
public:

	void* write(void* target, ParameterId parameterId) override;

	void add(ParameterId parameterId, ParameterWriterSharedPtr writer);

private:

	typedef std::unordered_map<ParameterId, ParameterWriterSharedPtr> Writers;

	Writers writers_;

};

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_PARAMETERWRITERLIST_HPP_ */
