//
//  Logs.hpp
//  Maze
//
//  Created by Jaraxus on 09/07/2020.
//

#ifndef Logs_h
#define Logs_h

#include <algorithm>
#include <mutex>
#include <ostream>
#include <string>

#include <iostream>

#include "../../Config.hpp"

#ifdef MAZE_PLATFORM_WINDOWS
#   include <windows.h>
#endif

#if defined(MAZE_PLATFORM_WINDOWS) // TODO: add support on windows platform

#   error not supported yet

#elif defined(MAZE_PLATFORM_LINUX) || defined(MAZE_PLATFORM_APPLE)

#   define MAZE_LOG_COLOR_RED   "\x1B[31m"
#   define MAZE_LOG_COLOR_GREEN   "\x1B[32m"
#   define MAZE_LOG_COLOR_YELLOW   "\x1B[33m"
#   define MAZE_LOG_COLOR_BLUE   "\x1B[34m"
#   define MAZE_LOG_COLOR_MAGENTA   "\x1B[35m"
#   define MAZE_LOG_COLOR_CYAN   "\x1B[36m"
#   define MAZE_LOG_COLOR_WHITE   "\x1B[37m"
#   define MAZE_LOG_COLOR_DEFAULT "\x1B[0m"

#endif

# define MAZE_LOGS_GET_FILE_LOCATION std::string{__FILE__} + std::string{":"} + std::to_string(__LINE__)

namespace mz {

enum LogMessageType {
    ERROR,
    WARNING,
    SUCCESS,
    NEUTRAL,
    CUSTOM
};

class Logs {
public:
    Logs(std::ostream& stream, bool allowColorizedOutput);
    Logs(Logs const& log) = delete;
    Logs(Logs && log) = delete;
    
    Logs& operator=(Logs const& log) = delete;
    Logs& operator=(Logs && log) = delete;
    
    void display(std::string const& text, LogMessageType type, std::string && fileLocation = "", std::string && logColor = "");
    
    /**
     \param fileLocation optional location from where the log is emitted
     \param LogColor color to apply to message in case of custom log message type
     \warning LogColor is unused if type is not custom type
     */
    void display(std::string const& text, LogMessageType type, bool displayLogMessageType, bool displayTime, std::string && fileLocation = "", bool displayAbsolutePath = false, std::string && LogColor = "");
    
public:
    static Logs Global;
    
private:
#if defined(MAZE_PLATFORM_WINDOWS)
    void changeOutputColorWindows(LogMessageType type, std::string && LogColor = "");
#elif defined(MAZE_PLATFORM_LINUX) || defined(MAZE_PLATFORM_APPLE)
    void changeOutputColorPosix(LogMessageType type, std::string && LogColor = "");
#endif
    
private:
    bool            m_allowColorizedOutput;
    std::mutex      m_mutex;
    std::ostream&   m_outputStream;
};

}

#endif /* Logs_h */
