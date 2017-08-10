structure.executable_project("coconut-shell", true)

project "coconut-shell"
	debugdir("../coconut-assets")
	links {
		"coconut-milk-graphics",
		"coconut-milk-fs",
		"coconut-milk-system",
		"coconut-pulp-math",
		"coconut-pulp-mesh",
		"coconut-pulp-primitive",
		"coconut-pulp-renderer",
		"coconut-pulp-world"
		}
	link_coconut_tools_libs({ "logger", "configuration", "serialisation", "system", "exceptions" })
project "*"
