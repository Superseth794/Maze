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

void mz::Logs::display(std::string const& text, LogMessageType type, bool displayLogMessageType, bool displayTime, std::string && fileLocation, bool displayAbsolutePath, std::string && logColor) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (displayTime) { // 2021-06-30 -- 2015/Mar/Sun[4]
        // TODO: class to encapsulate chrono
//        m_outputStream << date::year_month_day{date::floor<date::days>(std::chrono::system_clock::now())} << " " << __TIME__ << (!fileLocation.empty() ? " ": " : ");
        m_outputStream << "-- time display not fixed yet --" << std::endl;
    }
    
    if (!fileLocation.empty()) {
        if (!displayAbsolutePath)
            fileLocation.erase(fileLocation.begin(), fileLocation.begin() + fileLocation.rfind("Maze/")); // TODO: place in config constant
        m_outputStream << fileLocation << ": ";
    }
    
    if (m_allowColorizedOutput) {
#if defined(MAZE_PLATFORM_WINDOWS)
        changeOutputColorWindows(type, std::forward<std::string>(logColor));
#elif defined(MAZE_PLATFORM_LINUX) || defined(MAZE_PLATFORM_APPLE)
        changeOutputColorPosix(type, std::forward<std::string>(logColor));
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

void mz::Logs::changeOutputColorWindows(LogMessageType type, std::string && logColor) {
    
}

#elif defined(MAZE_PLATFORM_LINUX) || defined(MAZE_PLATFORM_APPLE)

void mz::Logs::changeOutputColorPosix(LogMessageType type, std::string && logColor) {
    if (isBeingDebugged()) // if an AppleCLang debugger is being used (Xcode) colorized output is disable because XCode console doesn't support colors
        return;
    
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
            if (logColor != "")
                m_outputStream << logColor;
            else
                m_outputStream << MAZE_LOG_COLOR_DEFAULT;
            break;
    }
}

#endif
