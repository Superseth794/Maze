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

	location "Debug_premake"

	files {
		"../Maze/Physics/**.hpp",
		"../Maze/Physics/**.cpp",
		"../Maze/Utils/**.hpp",
		"../Maze/Utils/**.cpp",
		"../Maze/Entity/**.hpp",
		"../Maze/Entity/**.cpp",
		"../Maze/Console.hpp",
		"../Maze/Console.cpp",
		"../Maze/Maze.hpp",
		"../Maze/Maze.cpp",
		"../Maze/main.cpp",
		-- "./Resources/**.*"
	}

	includedirs "../Maze/External/Catch/Catch.hpp"

	links {
		"../SFML/sfml-window.framework",
		"../SFML/sfml-graphics.framework",
		"../SFML/sfml-system.framework"
	}