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

	if os.execute("gcc -v") == 0 then
		toolset "gcc"
	end

	buildoptions {
		"-pedantic",
        "-pedantic-errors",
        "-Wall",
        "-Wextra",

        "-Warray-bounds",
        "-Wcast-align",
        "-Wcast-qual",
        "-Wconditionally-supported",
        "-Wconversion",
        "-Wdisabled-optimization",
        "-Wdouble-promotion",
        "-Wfloat-conversion",
        "-Wformat=2",
        "-Wformat-security",
        "-Wlogical-op",
        "-Wmissing-declarations",
        "-Wmissing-include-dirs",
        "-Wnoexcept",
        "-Wnon-virtual-dtor",
        "-Wold-style-cast",
        "-Wopenmp-simd",
        "-Woverloaded-virtual",
        "-Wpacked",
        "-Wredundant-decls",
        "-Wstrict-aliasing",
        "-Wstrict-null-sentinel",
        "-Wsuggest-final-methods",
        "-Wsuggest-final-types",
        "-Wtrampolines",
        "-Wundef",
        "-Wuninitialized",
        "-Wunused-macros",
        "-Wuseless-cast",
        "-Wvector-operation-performance",
        "-Wvla",
        "-Wzero-as-null-pointer-constant",
        "-Wno-comment",
        "-Wno-format-nonliteral"
	}

	files {
		"../Maze/**.cpp",
		"../Maze/**.hpp",
		"../Maze/**.inl"
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