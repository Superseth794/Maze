workspace "Maze"
	configurations { "Debug", "Release" }

	architecture "x86_64"

	filter "configurations:Debug"
		defines { "DEBUG" }

	filter "configurations:Release"
		defines { "NDEBUG" }

project "Maze"
	kind "ConsoleApp"

	language "C++"
	cppdialect("C++17")

	files {
		"../Maze/**.cpp",
		"../Maze/**.hpp"
	}

	includedirs {
		"../SFML/include"
	}

	links {
		"sfml-graphics",
		"sfml-window",
		"sfml-system"
	}

	libdirs {
		"../SFML/lib"
	}