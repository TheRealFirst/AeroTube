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

group "Core"
	include "Engine/Engine.lua"
group ""

group "Application"
	include "AeroTube/AeroTube.lua"
group ""