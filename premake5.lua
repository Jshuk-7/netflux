workspace "netflux"
	architecture "x64"
	startproject "testbed"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile",
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "netflux"
	location "netflux"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	defines
	{
        "_CRT_SECURE_NO_WARNINGS",
        "_WINSOCK_DEPRECATED_NO_WARNINGS",
	}

	includedirs
	{
		"%{prj.name}/src",
	}

	links
	{
    	"ws2_32.lib",
	}

	filter "system:windows"
		systemversion "latest"

		defines {}

	filter "configurations:Debug"
		defines "NF_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "NF_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "NF_DIST"
		runtime "Release"
        optimize "on"


project "testbed"
	location "testbed"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"netflux/src",
	}

	links
	{
    	"netflux",
	}

	filter "system:windows"
		systemversion "latest"

		defines {}

	filter "configurations:Debug"
		defines "NF_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "NF_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "NF_DIST"
		runtime "Release"
		optimize "on"