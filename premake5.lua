workspace "AeroTube"
	architecture "x64"
	startproject "AeroTube"
	
	configurations{
		"Debug",
		"Release",
		"Dist"
	}

	flags{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["yaml_cpp"] = "%{wks.location}/Engine/vendor/yaml-cpp/include"
IncludeDir["GLFW"] = "%{wks.location}/Engine/vendor/GLFW/include"
IncludeDir["glad"] = "%{wks.location}/Engine/vendor/glad/include"
IncludeDir["glm"] = "%{wks.location}/Engine/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/Engine/vendor/stb_image"


group "Dependencies"
	include "vendor/premake"
	include "Engine/vendor/yaml-cpp"
	include "Engine/vendor/GLFW"
	include "Engine/vendor/glad"
group ""

group "Core"
	include "Engine"
group ""

group "Application"
	include "AeroTube"
group ""