project "Engine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "atpch.h"
	pchsource "src/atpch.cpp"

	files {
		"src/**.h",
		"src/**.cpp"
	}

	defines{
		"GLFW_INCLUDE_NONE",
		"YAML_CPP_STATIC_DEFINE",
	}

	includedirs{
		"src",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glad}",
	}
	

	links{
		"yaml-cpp",
		"GLFW",
		"glad",
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines{
			"PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "AT_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "AT_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "AT_DEBUG"
		optimize "On"