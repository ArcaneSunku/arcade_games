workspace "ArcadeClassics"
	filename "ArcadeClassics"
	architecture "x64"
	configurations {"Debug", "Release", "Distribution"}
	
	startproject "ArcadeClones"
	
	OUTPUT_DIR = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
	
	APP_INCLUDE_DIR = {}
	APP_INCLUDE_DIR["GLM"]         = "ArcadeClones/vendors/Arcane/Arcane/vendors/glm"
	APP_INCLUDE_DIR["SPD_LOG"]     = "ArcadeClones/vendors/Arcane/Arcane/vendors/spdlog/include"
	APP_INCLUDE_DIR["IMGUI"]       = "ArcadeClones/vendors/Arcane/Arcane/vendors/imgui"
	APP_INCLUDE_DIR["OpenAL"]      = "ArcadeClones/vendors/Arcane/Arcane/vendors/openal-soft/include"
	APP_INCLUDE_DIR["libsndfile"]  = "ArcadeClones/vendors/Arcane/Arcane/vendors/libsndfile/include"
	APP_INCLUDE_DIR["ARCANE"]      = "ArcadeClones/vendors/Arcane/Arcane/src"
	
	group "Dependencies"
		include "ArcadeClones/vendors/Arcane/Arcane/vendors/glfw"
		include "ArcadeClones/vendors/Arcane/Arcane/vendors/glad"
		include "ArcadeClones/vendors/Arcane/Arcane/vendors/imgui"
		include "ArcadeClones/vendors/Arcane/Arcane/vendors/openal-soft"

		include "ArcadeClones/vendors/Arcane"
	
	group ""
	
project "ArcadeClones"
	location "ArcadeClones"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"
	
	buildoptions{"/utf-8"}
	
	targetdir("bin/" .. OUTPUT_DIR .. "/%{prj.name}")
	objdir("bin-int/" .. OUTPUT_DIR .. "/%{prj.name}")
	
	files 
	{
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.c",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.h"
	}
	
	includedirs 
	{
		"%{APP_INCLUDE_DIR.SPD_LOG}",
		"%{APP_INCLUDE_DIR.IMGUI}",
		"%{APP_INCLUDE_DIR.OpenAL}",
		"%{APP_INCLUDE_DIR.libsndfile}",
		"%{APP_INCLUDE_DIR.GLM}",
		"%{APP_INCLUDE_DIR.ARCANE}"
	}
	
	links {
		"Arcane"
	}
	
	filter "system:windows"
		systemversion "latest"
		
	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
		
	filter "configurations:Release"
		runtime "Release"
		optimize "on"
		
	filter "configurations:Distribution"
		runtime "Release"
		optimize "on"
		