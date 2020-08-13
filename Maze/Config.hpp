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

#if defined(MAZE_DEBUG) && defined(MAZE_PLATFORM_APPLE) && defined(__apple_build_version__)

#   include <assert.h>
#   include <stdbool.h>
#   include <sys/types.h>
#   include <unistd.h>
#   include <sys/sysctl.h> // this header will only compile with AppleClang compiler (XCode)

// Returns true if the current process is being debugged (either running under the debugger or has a debugger attached post facto).
// Code is integrally taken from https://developer.apple.com/library/archive/qa/qa1361/_index.html
[[maybe_unused]] static bool isBeingDebugged() {
    int                 junk;
    int                 mib[4];
    struct kinfo_proc   info;
    size_t              size;

    // Initialize the flags so that, if sysctl fails for some bizarre
    // reason, we get a predictable result.

    info.kp_proc.p_flag = 0;

    // Initialize mib, which tells sysctl the info we want, in this case
    // we're looking for information about a specific process ID.

    mib[0] = CTL_KERN;
    mib[1] = KERN_PROC;
    mib[2] = KERN_PROC_PID;
    mib[3] = getpid();

    // Call sysctl.

    size = sizeof(info);
    junk = sysctl(mib, sizeof(mib) / sizeof(*mib), &info, &size, nullptr, 0);
    assert(junk == 0);

    // We're being debugged if the P_TRACED flag is set.

    return ( (info.kp_proc.p_flag & P_TRACED) != 0 );
}

#else

// unable to detect if a debugger is currently beging used
static constexpr bool isBeingDebugged() {
    return false;
}

#endif

#endif /* Config_h */
