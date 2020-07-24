//
//  Logs.cpp
//  Maze
//
//  Created by Jaraxus on 14/07/2020.
//

#include "../../include/Core/Logs.hpp"

mz::Logs::Logs(std::ostream& stream, bool allowColorizedOutput) :
m_allowColorizedOutput(allowColorizedOutput),
m_outputStream(stream)
{}

void mz::Logs::display(std::string const& text, LogMessageType type, std::string && fileLocation, std::string && logColor) {
    display(text, type, true, true, std::forward<std::string>(fileLocation), false, std::forward<std::string>(logColor));
}

void mz::Logs::display(std::string const& text, LogMessageType type, bool displayLogMessageType, bool displayTime, std::string && fileLocation, bool displayAbsolutePath, std::string && LogColor) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (displayTime) {
        m_outputStream << __DATE__ << " " << __TIME__ << (!fileLocation.empty() ? " ": " : ");
    }
    
    if (!fileLocation.empty()) {
        if (!displayAbsolutePath)
            fileLocation.erase(fileLocation.begin(), fileLocation.begin() + fileLocation.rfind("Maze/"));
        m_outputStream << fileLocation << ": ";
    }
    
    if (m_allowColorizedOutput) {
#if defined(MAZE_PLATFORM_WINDOWS)
        changeOutputColorWindows(type, std::forward<std::string>(LogColor));
#elif defined(MAZE_PLATFORM_LINUX) || defined(MAZE_PLATFORM_APPLE)
        changeOutputColorPosix(type, std::forward<std::string>(LogColor));
#endif
    }
    
    if (displayLogMessageType) {
        switch (type) {
            case ERROR:
                m_outputStream << "Error   - ";
                break;
            case WARNING:
                m_outputStream << "Warning - ";
                break;
            case SUCCESS:
                m_outputStream << "Success - ";
                break;
            default:
                break;
        }
    }
    
    m_outputStream << text;
    
    if (m_allowColorizedOutput) {
#if defined(MAZE_PLATFORM_WINDOWS)
        changeOutputColorWindows(LogMessageType::CUSTOM, MAZE_LOG_COLOR_DEFAULT);
#elif defined(MAZE_PLATFORM_LINUX) || defined(MAZE_PLATFORM_APPLE)
        changeOutputColorPosix(LogMessageType::CUSTOM, MAZE_LOG_COLOR_DEFAULT);
#endif
    }
    
    m_outputStream << "\n";
}

#if defined(MAZE_PLATFORM_WINDOWS)

void mz::Logs::changeOutputColorWindows(LogMessageType type, std::string &&LogColor) {
    
}

#elif defined(MAZE_PLATFORM_LINUX) || defined(MAZE_PLATFORM_APPLE)

void mz::Logs::changeOutputColorPosix(LogMessageType type, std::string && LogColor) {
    switch (type) {
        case ERROR:
            m_outputStream << MAZE_LOG_COLOR_RED;
            break;
        case WARNING:
            m_outputStream << MAZE_LOG_COLOR_YELLOW;
            break;
        case SUCCESS:
            m_outputStream << MAZE_LOG_COLOR_GREEN;
            break;
        case NEUTRAL:
            m_outputStream << MAZE_LOG_COLOR_DEFAULT;
            break;
        case CUSTOM:
            if (LogColor != "")
                m_outputStream << LogColor;
            else
                m_outputStream << MAZE_LOG_COLOR_DEFAULT;
            break;
    }
}

#endif
