include "premake5.cfg.lua"

structure = require "structure"

include "boost.lua"
include "gmock.lua"
include "coconut-tools.lua"

workspace "coconut"

	-- configurations { "DebugStatic", "DebugShared", "ReleaseStatic", "ReleaseShared" }
	configurations { "DebugStatic", "ReleaseStatic" } -- dllexport not set up
	
	platforms { "Win64" }
	
	language "C++"

	location "build"
	
	-- Configuration specific settings
	
	filter "configurations:Debug*"
		symbols "On"
		
	filter "configurations:Release*"
		defines { "NDEBUG" }
		optimize "On"
	
	-- Platform specific settings
	
	filter "platforms:Win32"
		system "Windows"
		architecture "x32"

	filter "platforms:Win64"
		system "Windows"
		architecture "x64"
		
	filter {}
	
	-- Global includes
	
	includedirs { USR_DIR.."/include" }
	includedirs { BOOST_INCLUDE_DIR }
	
	-- Dependency libraries
	
	libdirs { USR_DIR.."/lib" }
	libdirs { BOOST_LIB_DIR }
	
	-- Warnings
	
	warnings "Extra"
	flags { "FatalWarnings" }
	
	filter "action:vs*"
		defines { "_SCL_SECURE_NO_WARNINGS" }
	filter {}
	
	include "coconut-milk-fs"
	include "coconut-milk-graphics-dx11"
	include "coconut-milk-system-win32"
	include "coconut-milk-utils"
	include "coconut-pulp-math"
	-- include "coconut-pulp-mesh"
	-- include "coconut-pulp-primitive"
	-- include "coconut-pulp-renderer"
	-- include "coconut-pulp-world"
	-- include "coconut-pulp-shell"

	structure.create_install_project()
	structure.create_run_tests_project()
	