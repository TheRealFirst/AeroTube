project "Engine"
	kind "StaticLib"
	language "C++"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "atpch.h"
	pchsource "src/atpch.cpp"

	files {
		"src/**.h",
		"src/**.cpp"
	}

	includedirs{

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