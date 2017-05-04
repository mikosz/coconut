#ifndef _COCONUT_PULP_RENDERER_SHADER_PASS_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_PASS_HPP_

#include "Input.hpp"
#include "Shader.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class PassContext;
	
namespace shader {

class Pass {
public:

	Pass(
		bool isInstanced,
		InputSharedPtr input,
		VertexShaderSharedPtr vertexShader,
		GeometryShaderSharedPtr geometryShader,
		HullShaderSharedPtr hullShader,
		DomainShaderSharedPtr domainShader,
		PixelShaderSharedPtr pixelShader
		) :
		isInstanced_(isInstanced),
		input_(std::move(input)),
		vertexShader_(std::move(vertexShader)),
		geometryShader_(std::move(geometryShader)),
		hullShader_(std::move(hullShader)),
		domainShader_(std::move(domainShader)),
		pixelShader_(std::move(pixelShader))
	{
	}

	void bind(DrawCommand& drawCommand, const Properties& properties) const;

	const Input& input() const noexcept {
		return *input_;
	}

	Input& input() noexcept {
		return *input_;
	}

	bool isInstanced() const noexcept {
		return isInstanced_;
	}

	VertexShader& vertexShader() noexcept {
		return *vertexShader_;
	}

	bool hasGeometryShader() const noexcept {
		return !!geometryShader_;
	}

	GeometryShader& geometryShader() noexcept {
		return *geometryShader_;
	}

	bool hasHullShader() const noexcept {
		return !!hullShader_;
	}

	HullShader& hullShader() noexcept {
		return *hullShader_;
	}

	bool hasDomainShader() const noexcept {
		return !!domainShader_;
	}

	DomainShader& domainShader() noexcept {
		return *domainShader_;
	}

	PixelShader& pixelShader() noexcept {
		return *pixelShader_;
	}

private:

	bool isInstanced_;

	InputSharedPtr input_;

	VertexShaderSharedPtr vertexShader_;

	GeometryShaderSharedPtr geometryShader_;

	HullShaderSharedPtr hullShader_;
	
	DomainShaderSharedPtr domainShader_;

	PixelShaderSharedPtr pixelShader_;

};

CCN_MAKE_POINTER_DEFINITIONS(Pass);

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_PASS_HPP_ */
