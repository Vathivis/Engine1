workspace "IPS_demo_v2"
	architecture "x86"
	

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

	flags {
		"MultiProcessorCompile"
	}

	startproject "Application"
	

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
includeDir = {}
includeDir["GLFW"] = "Engine1/vendor/GLFW/include"
includeDir["Glad"] = "Engine1/vendor/Glad/include"
includeDir["ImGui"] = "Engine1/vendor/imgui"
includeDir["glm"] = "Engine1/vendor/glm"
includeDir["stb_image"] = "Engine1/vendor/stb_image"


group "Dependencies"
	include "Engine1/vendor/GLFW"
	include "Engine1/vendor/Glad"
	include "Engine1/vendor/imgui"
group ""


project	"Engine1"
	location "Engine1"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "E1pch.h"
	pchsource "Engine1/src/E1pch.cpp"

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	defines {
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs {
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{includeDir.GLFW}",
		"%{includeDir.Glad}",
		"%{includeDir.ImGui}",
		"%{includeDir.glm}",
		"%{includeDir.stb_image}"
	}

	links { 
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines {
			"E1_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	
	filter "configurations:Debug"
		defines "E1_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "E1_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "E1_DIST"
		runtime "Release"
		optimize "on"


project "Application"
	location "Application"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

		files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	--glad is temporary
	includedirs {
		"Engine1/vendor/spdlog/include",
		"Engine1/src",
		"Engine1/vendor",
		"%{includeDir.Glad}",
		"%{includeDir.glm}"
	}

	links {
		"Engine1",
		"Glad"
	}

	filter "system:windows"
		systemversion "latest"
	
	filter "configurations:Debug"
		defines "E1_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "E1_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "E1_DIST"
		runtime "Release"
		optimize "on"
