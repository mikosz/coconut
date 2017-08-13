include "directx-tex.lua"

local common = function()
	includedirs { directx_tex_include_dir() }
	link_coconut_tools_libs { "exceptions", "system" }
	link_directx_tex_libs()
end

structure.library_project("coconut-milk-graphics", common)

project "coconut-milk-graphics-unit-test"
	add_gmock_files()
