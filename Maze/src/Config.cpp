//
//  Config.cpp
//  Maze
//
//  Created by Jaraxus on 04/05/2021.
//

#include "../include/Config.hpp"

#if defined(MAZE_DEBUG) && defined(MAZE_PLATFORM_APPLE) && defined(MAZE_COMPILER_CLANG) && defined(__apple_build_version__)

#   include <assert.h>
#   include <stdbool.h>
#   include <sys/types.h>
#   include <unistd.h>
#   include <sys/sysctl.h> // this header will only compile with AppleClang compiler (XCode)

// Code was originally found in https://github.com/catchorg/Catch2/blob/devel/src/catch2/internal/catch_debugger.cpp
// Returns true if the current process is being debugged (either running under the debugger or has a debugger attached post facto).
// Code is integrally taken from the technical note https://developer.apple.com/library/archive/qa/qa1361/_index.html
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

#endif
