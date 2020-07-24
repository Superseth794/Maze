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

#endif /* Config_h */
