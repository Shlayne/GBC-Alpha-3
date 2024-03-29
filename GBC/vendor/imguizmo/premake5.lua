project "imguizmo"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"include/imguizmo/**.h",
		"src/**.cpp"
	}

	includedirs {
		"include/imguizmo",
		"%{includedir.imgui}/imgui",
		"%{wks.location}/GBC/src",

		"%{includedir.spdlog}",

		"%{includedir.glm}"
	}

	filter "system:windows"
		systemversion "latest"
		defines "GBC_PLATFORM_WINDOWS"

	filter "configurations:Debug"
		defines "GBC_CONFIG_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "GBC_CONFIG_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "GBC_CONFIG_DIST"
		runtime "Release"
		optimize "on"
