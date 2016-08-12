#ifndef _COCONUT_MILK_GRAPHICS_SHADERREFLECTION_HPP_
#define _COCONUT_MILK_GRAPHICS_SHADERREFLECTION_HPP_

#include <string>
#include <vector>

#include <coconut-tools/enum.hpp>

#include "coconut/milk/system/COMWrapper.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class ShaderReflection {
public:

	CCN_MEMBER_ENUM(
		InputParameterSemantic,
		(POSITION)
		(TEXCOORD)
		(NORMAL)
		);

	struct InputParameterInfo {

		InputParameterSemantic semantic;

		size_t semanticIndex;

	};

	using InputParameterInfos = std::vector<InputParameterInfo>;

	struct ConstantBufferInfo {

		struct Variable {
		
			std::string name;

			size_t offset;

			size_t size;

		};

		using Variables = std::vector<Variable>;

		size_t size;

		Variables variables;

	};

	using ConstantBufferInfos = std::vector<ConstantBufferInfo>;

	ShaderReflection(const void* shaderData, size_t shaderSize);

	const InputParameterInfos& inputParameters() const {
		return inputParameters_;
	}

	const ConstantBufferInfos& constantBuffers() const {
		return constantBuffers_;
	}

private:

	InputParameterInfos inputParameters_;

	ConstantBufferInfos constantBuffers_;

};

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_SHADERREFLECTION_HPP_ */
