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
		"src/**.cpp",
		"vendor/stb_image/**.h",
		"vendor/stb_image/**.cpp",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl",
		"vendor/json/json.h",
		"vendor/tinygltf/tiny_gltf.h",
		-- "vendor/tinygltf/json.hpp",
		-- "vendor/tinygltf/stb_image.h",
		"vendor/tinygltf/stb_image_write.h",
	}

	defines{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE",
		"YAML_CPP_STATIC_DEFINE",
	}

	includedirs{
		"src",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.json}",
		"%{IncludeDir.tinygltf}",
	}
	

	links{
		"yaml-cpp",
		"GLFW",
		"glad",
		"ImGui",
		"opengl32.lib",
		-- "tinygltf.lib",
	}

	

	filter "system:windows"
		systemversion "latest"

		defines{
			"PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "AT_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "AT_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "AT_DIST"
		runtime "Release"
		optimize "On"