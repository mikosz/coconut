#ifndef _COCONUT_MILK_GRAPHICS_SHADERREFLECTION_HPP_
#define _COCONUT_MILK_GRAPHICS_SHADERREFLECTION_HPP_

#include <string>
#include <vector>
#include <tuple>

#include <d3dcommon.h>
#include "coconut/milk/system/cleanup-windows-macros.hpp"

#include <coconut-tools/enum.hpp>

#include "coconut/milk/system/COMWrapper.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class ShaderReflection {
public:

	struct InputParameterInfo {

		CCN_MEMBER_ENUM(
			Semantic,
			(POSITION)
			(SV_POSITION)
			(TEXCOORD)
			(NORMAL)
			);

		CCN_MEMBER_ENUM_VALUES(
			DataType,
			(FLOAT)(D3D_REGISTER_COMPONENT_FLOAT32)
			(UINT)(D3D_REGISTER_COMPONENT_UINT32)
			(INT)(D3D_REGISTER_COMPONENT_SINT32)
			);

		Semantic semantic;

		size_t semanticIndex;

		DataType dataType;

		size_t elements;

	};

	using InputParameterInfos = std::vector<InputParameterInfo>;

	struct Type {

		using Member = std::tuple<std::string, Type>;

		static const size_t MemberNameTag = 0;

		static const size_t MemberTypeTag = 1;

		using Members = std::vector<Member>;

		CCN_MEMBER_ENUM_VALUES(
			Class,
			(SCALAR)(D3D_SVC_SCALAR)
			(VECTOR)(D3D_SVC_VECTOR)
			(MATRIX_ROW_MAJOR)(D3D_SVC_MATRIX_ROWS)
			(MATRIX_COLUMN_MAJOR)(D3D_SVC_MATRIX_COLUMNS)
			(OBJECT)(D3D_SVC_OBJECT)
			(STRUCT)(D3D_SVC_STRUCT)
			);

		CCN_MEMBER_ENUM_VALUES(
			ScalarType,
			(VOID)(D3D_SVT_VOID)
			(BOOL)(D3D_SVT_BOOL)
			(INT)(D3D_SVT_INT)
			(UINT)(D3D_SVT_UINT)
			(FLOAT)(D3D_SVT_FLOAT)
			);

		std::string name;

		size_t offset;

		Class klass;

		ScalarType scalarType;

		size_t elements;

		Members members;

	};

	struct Variable {

		Type type;

		std::string name;

		size_t offset;

		size_t size;

	};

	struct ConstantBufferInfo {

		using Variables = std::vector<Variable>;

		size_t size;

		size_t slot;

		Variables variables;

	};

	using ConstantBufferInfos = std::vector<ConstantBufferInfo>;

	struct ResourceInfo {

		CCN_MEMBER_ENUM_VALUES(
			Type,
			(SAMPLER)(D3D_SIT_SAMPLER)
			(TEXTURE)(D3D_SIT_TEXTURE)
			);

		Type type;

		std::string name;

		size_t slot;

		size_t dimensions;

	};

	using ResourceInfos = std::vector<ResourceInfo>;

	ShaderReflection(const void* shaderData, size_t shaderSize);

	const InputParameterInfos& inputParameters() const {
		return inputParameters_;
	}

	const ConstantBufferInfos& constantBuffers() const {
		return constantBuffers_;
	}

	const ResourceInfos& resources() const {
		return resources_;
	}

private:

	InputParameterInfos inputParameters_;

	ConstantBufferInfos constantBuffers_;

	ResourceInfos resources_;

};

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_SHADERREFLECTION_HPP_ */
