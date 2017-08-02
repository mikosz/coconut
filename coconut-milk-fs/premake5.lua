local fs_common = function()
	link_coconut_tools_libs { "exceptions", "system" }
end

structure.library_project("coconut-milk-fs", fs_common)

project "coconut-milk-fs-unit-test"
	add_gmock_files()
