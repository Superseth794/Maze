//
//  Config.hpp
//  Maze
//
//  Created by Jaraxus on 09/07/2020.
//

#ifndef Config_h
#define Config_h

#define MAZE_MAJOR_VERSION 0
#define MAZE_MINOR_VERSION 1
#define MAZE_PATCH_VERSION 0

#if defined(_WIN32) || defined(__WIN32__)
#   define MAZE_PLATFORM_WINDOWS
#elif defined(linux) || defined(__linux__) || defined(__linux)
#   define MAZE_PLATFORM_LINUX
#elif defined(__APPLE__) || defined(__MACH__)
#   define MAZE_PLATFORM_APPLE
#else
#   error Platform not suported
#endif

#if !defined(NDEBUG)
#   define MAZE_DEBUG
#endif

#if defined(_MSC_VER)
#   define MAZE_COMPILER_MSVC
#elif defined(__GNUC__)
#   define MAZE_COMPILER_GCC
#elif defined(__clang__)
#   define MAZE_COMPILER_CLANG
#elif defined (__EMSCRIPTEN__)
#   define MAZE_COMPILER_EMSCRIPTEN
#elif defined (__MINGW32__) || defined(__MINGW32__) || defined(__MINGW64__)
#   define MAZE_COMPILER_MINGW
#else
#   error Compiler not supported
#endif

#if defined(MAZE_COMPILER_GCC)
#   define MAZE_STDLIB_LIBSTDCPP
#elif defined(MAZE_COMPILER_CLANG)
#   include <ciso646>
#   include <c++config.h,>
#   if defined(_GLIBCXX_)
#       define MAZE_LIBSTD_LIBSTDCPP
#   elif defined(_LIBCPP_VERSION)
#       define MAZE_LIBSTD_LIBCPP
#   else
#       error Unsupported cpp standart lib for clang compiler
#   endif
#else
#   error Unsuported cpp standart lib
#endif

#if defined(MAZE_DEBUG) && defined(MAZE_PLATFORM_APPLE) && defined(__apple_build_version__)
[[maybe_unused]] bool isBeingDebugged();
#else
static constexpr bool isBeingDebugged() {return false;}; // unable to detect if a debugger is currently beging used
#endif

#if defined MAZE_DEBUG
#   define MAZE_SYSTEM_OBJECT_POOL_MEMORY_SAFETY
#endif

#endif /* Config_h */
