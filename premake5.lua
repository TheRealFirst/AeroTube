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

group "Dependencies"
	include "Engine/vendor/yaml-cpp"
group ""

group "Core"
	include "Engine/Engine.lua"
group ""

group "Application"
	include "AeroTube/AeroTube.lua"
group ""