#ifndef _COCONUT_MILK_GRAPHICS_DX11_COMPILESHADER_HPP_
#define _COCONUT_MILK_GRAPHICS_DX11_COMPILESHADER_HPP_

#include <d3dcommon.h>

#include <functional>
#include <vector>
#include <cstddef>
#include <memory>
#include <iosfwd>
#include <string>

#include <d3dcompiler.h>
#include "coconut/milk/system/cleanup-windows-macros.hpp"

#include <coconut-tools/enums/Mask.hpp>

#include "coconut/milk/system/COMWrapper.hpp"

#include "ShaderType.hpp"

namespace coconut {
namespace milk {
namespace graphics {

class ShaderCompiler {
public:

    using ShaderData = const std::vector<std::uint8_t>;
    using IncludeHandler = std::function<std::shared_ptr<ShaderData>(const std::string&)>;

    CT_MEMBER_FLAG_VALUES(
        CompilerFlag,
        (DEBUG)(D3DCOMPILE_DEBUG)
        (SKIP_OPTIMISATION)(D3DCOMPILE_SKIP_OPTIMIZATION)
        );

    using CompilerFlags = coconut_tools::Mask<CompilerFlag>;

    ShaderCompiler(CompilerFlags globalFlags = CompilerFlags()) :
        globalCompilerFlags_(globalFlags)
    {
    }

    ShaderData compile(
    	const std::vector<std::uint8_t>& code,
        const std::string& name,
	    const std::string& entrypoint,
    	ShaderType type,
        IncludeHandler includeHandler,
        CompilerFlags instanceFlags = CompilerFlags()
        );

private:

    CompilerFlags globalCompilerFlags_;

};

} // namespace graphics
} // namespace milk
} // namespace coconut

#endif /* _COCONUT_MILK_GRAPHICS_DX11_COMPILESHADER_HPP_ */
