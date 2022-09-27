workspace "Sora"
	architecture "x64"

	configurations{
		"Debug",
		"Release"
	}


outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "GameEngine"
	location "GameEngine"
	kind "StaticLib"
	language "C++"

	targetdir ("%{wks.location}/OutputOfAll/")
	objdir ("%{wks.location}/bin-int/" .. outputDir .. "/%{prj.name}")

	files{
		
		"%{prj.name}/src/glad.c",
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"

	}

	removefiles {

		"imgui_main.cpp",
		"%{prj.name}/src/ImGUIStuff/imgui_main.cpp",
		"%{prj.name}/src/CPPFiles/Systems/TemplateSystem.cpp",
		"%{prj.name}/src/HelpHeaders/Systems/TemplateSystem.h",
		"%{prj.name}/src/imgui.ini",
		"%{prj.name}/src/assimp-vc142-mtd.dll",

	}

	includedirs{
		
		"vendors/Include/"

	}

	links{
		"glfw3",
		"opengl32",
		"assimp-vc142-mtd"
	}

	libdirs {
		"vendors/Lib/"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines{

			"SORA_PLATFORM_WINDOWS",
			"_WINDLL"
		}

		filter "configurations:debug"
			defines "SORA_DEBUG"
			symbols "On"

		filter "configurations:Release"
			defines "SORA_RELEASE"
			optimize "On"

		filter "system:windows"
			buildoptions "/MD"



project "Sandbox"
	location "Sandbox"
	kind "SharedLib"
	language "C++"

	targetdir ("%{wks.location}/OutputOfAll/")
	objdir ("%{wks.location}/bin-int/" .. outputDir .. "/%{prj.name}")

	files{

		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"

	}

	includedirs{
		
		"vendors/Include/",
		"GameEngine/src",
		"GameEngine/src/HelpHeaders"
	}

	links{
		"glfw3",
		"opengl32",
		"assimp-vc142-mtd",
		"GameEngine"
	}

	libdirs {
		"vendors/Lib/"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines{

			"SORA_PLATFORM_WINDOWS",
			"_WINDLL"
		}

		filter "configurations:debug"
			defines "SORA_DEBUG"
			symbols "On"

		filter "configurations:Release"
			defines "SORA_RELEASE"
			optimize "On"

		filter "system:windows"
			buildoptions "/MD"


			
project "Executable"
	location "Executable"
	kind "ConsoleApp"
	language "C++"

	targetdir ("%{wks.location}/OutputOfAll/")
	objdir ("%{wks.location}/bin-int/" .. outputDir .. "/%{prj.name}")

	files{

		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"

	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines{

			"SORA_PLATFORM_WINDOWS",
			"_WINDLL"
		}

		filter "configurations:debug"
			defines "SORA_DEBUG"
			symbols "On"

		filter "configurations:Release"
			defines "SORA_RELEASE"
			optimize "On"

		filter "system:windows"
			buildoptions "/MD"
		
		