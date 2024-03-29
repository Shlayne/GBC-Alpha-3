project "Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"src/**.h",
		"src/**.inl",
		"src/**.cpp"
	}

	includedirs {
		"src",
		"%{wks.location}/GBC/src",

		"%{includedir.imgui}",
		"%{includedir.imguizmo}",
		"%{includedir.stb}",
		"%{includedir.yaml}",

		"%{includedir.entt}",
		"%{includedir.glm}",
		"%{includedir.siv}",
		"%{includedir.spdlog}"
	}

	links {
		"GBC"
	}

	filter "system:windows"
		systemversion "latest"
		defines "GBC_PLATFORM_WINDOWS"
-- Until Microsoft updates Windows 10 to not have terrible code, this must be here to prevent a warning
		buildoptions "/wd5105"

	filter "configurations:Debug"
		defines "GBC_CONFIG_DEBUG"
		runtime "Debug"
		symbols "on"

		postbuildcommands {
			"pushd \"%{librarydir.VulkanSDK_DebugDLL}\"",
			"copy *.dll \"%{wks.location}/bin/" .. outputdir .. "/%{prj.name}\"",
			"popd"
		}

	filter "configurations:Release"
		defines "GBC_CONFIG_RELEASE"
		runtime "Release"
		optimize "on"

		excludes "src/Panels/Debug/*.cpp"

	filter "configurations:Dist"
		defines "GBC_CONFIG_DIST"
		runtime "Release"
		optimize "on"

		excludes {
			"src/Panels/Debug/*.cpp",
			"src/Panels/Release/*.cpp"
		}

-- Remove the console in Dist on Windows
	filter { "configurations:Dist", "system:windows" }
		linkoptions "/SUBSYSTEM:windows /ENTRY:mainCRTStartup"
