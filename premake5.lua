workspace "IPS_demo_v2"
	architecture "x86"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}
	
	startproject "Application"

outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
includeDir = {}
includeDir["GLFW"] = "Engine1/vendor/GLFW/include"
includeDir["Glad"] = "Engine1/vendor/Glad/include"
includeDir["ImGui"] = "Engine1/vendor/imgui"
includeDir["glm"] = "Engine1/vendor/glm"

include "Engine1/vendor/GLFW"
include "Engine1/vendor/Glad"
include "Engine1/vendor/imgui"


project	"Engine1"
	location "Engine1"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

	targetdir("bin/" .. outputDir .. "/%{prj.name}")
	objdir("bin-int/" .. outputDir .. "/%{prj.name}")

	pchheader "E1pch.h"
	pchsource "Engine1/src/E1pch.cpp"

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	includedirs {
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{includeDir.GLFW}",
		"%{includeDir.Glad}",
		"%{includeDir.ImGui}",
		"%{includeDir.glm}"
	}

	links { 
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines {
			"E1_PLATFORM_WINDOWS",
			"E1_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands {
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputDir .. "/Application/\"")
		}
	
	filter "configurations:Debug"
		defines "E1_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "E1_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "E1_DIST"
		runtime "Release"
		optimize "On"


project "Application"
	location "Application"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir("bin/" .. outputDir .. "/%{prj.name}")
	objdir("bin-int/" .. outputDir .. "/%{prj.name}")

		files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"Engine1/vendor/spdlog/include",
		"Engine1/src",
		"%{includeDir.glm}"
	}

	links {
		"Engine1"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines {
			"E1_PLATFORM_WINDOWS",
		}

	
	filter "configurations:Debug"
		defines "E1_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "E1_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "E1_DIST"
		runtime "Release"
		optimize "On"