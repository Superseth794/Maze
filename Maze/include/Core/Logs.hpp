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

#   define MAZE_LOG_COLOR_RED       "\x1B[31m"
#   define MAZE_LOG_COLOR_GREEN     "\x1B[32m"
#   define MAZE_LOG_COLOR_YELLOW    "\x1B[33m"
#   define MAZE_LOG_COLOR_BLUE      "\x1B[34m"
#   define MAZE_LOG_COLOR_MAGENTA   "\x1B[35m"
#   define MAZE_LOG_COLOR_CYAN      "\x1B[36m"
#   define MAZE_LOG_COLOR_WHITE     "\x1B[37m"
#   define MAZE_LOG_COLOR_DEFAULT   "\x1B[0m"

#endif

#define MAZE_LOGS_GET_FILE_LOCATION std::string{__FILE__} + std::string{":"} + std::to_string(__LINE__)

namespace mz {

/**
 \enum LogMessageType
 \brief logs messages' types availables
 \details defines wich color will be used in case of colorized output. If the message type needs to be displayed, only error, warning and sucess types will have their names displayed.
 */
enum LogMessageType {
    ERROR, //> red output color
    WARNING, //> yellow output color
    SUCCESS, //> green output color
    NEUTRAL, //> system default output color
    CUSTOM //> user-defined output color
};

/**
 \class Logs
 \brief Logs class provides a thread-safe way to output logs messages to an output stream
 \details A Global Logs object is provided that can be accessed from anywhere. The object is not default-initialized and must be initialized in main.cpp according to its futur usages. New Logs objects can be created on any output streams. The output stream must remain valid as long as the Logs object is used
 */
class Logs {
public:
    /**
     \brief Constructor of Logs class
     \details A logs object is constructed over an output stream that will be used to output logs messages
     \param stream Output stream used to output logs messages. The stream must remain valid as long as the Logs object is used.
     \param allowColorizedOutput Defines if the output logs messages will be displayed with colors
     */
    Logs(std::ostream& stream, bool allowColorizedOutput);
    
    /**
     \brief Copy constructor is deleted
     */
    Logs(Logs const& log) = delete;
    
    /**
    \brief Move constructor is deleted
    */
    Logs(Logs && log) = delete;
    
    
    /**
    \brief Copy assignation operator is deleted
    */
    Logs& operator=(Logs const& log) = delete;
    
    /**
    \brief Move assignation operator is deleted
    */
    Logs& operator=(Logs && log) = delete;
    
    
    /**
     \brief Displays a given message to the log output stream
     \details The message is colorized if colorized input is allowed. The message is terminated by a new line. The following parameters are used :
     \li The message type is displayed
     \li The time when displaying the message is displayed
     \li If a fileLocation is provided, the relative path to the project is used
     \param text Message to display in logs
     \param type Type of the message
     \param fileLocation A string containning the location of the call to display the logs message. The formatted location can be obtained through the macro MAZE_LOGS_GET_FILE_LOCATION
     \param logColor Color to apply to the message in case of custom log message type
     \warning logColor is unused if type is not custom type
     */
    void display(std::string const& text, LogMessageType type, std::string && fileLocation = "", std::string && logColor = "");
    
    /**
     \brief Displays a given message to the log output stream
     \details The message is colorized if colorized input is allowed. The message is terminated by a new line
     \param text Message to display in logs
     \param type Type of the message
     \param displayLogMessageType Defines if the message's type should be appened to the output message
     \param displayTime Defines if the local time when the log request is called should be appended to the output message
     \param fileLocation A string containning the location of the call to display the logs message. The formatted location can be obtained through the macro MAZE_LOGS_GET_FILE_LOCATION
     \param displayAbsolutePath Defines if the file location (if any) should be displayed as an absolute path or as a path relative to the Maze/ directory storing all include and source files
     \param logColor Color to apply to the message in case of custom log message type
     \warning LogColor is unused if type is not custom type
    */
    void display(std::string const& text, LogMessageType type, bool displayLogMessageType, bool displayTime, std::string && fileLocation = "", bool displayAbsolutePath = false, std::string && logColor = "");
    
private:
#if defined(MAZE_PLATFORM_WINDOWS)
    /**
     \brief Implements the colorized output for windows platform
     \param type Type to use to get color
     \param logColor Color to apply to the message in case of custom log message type
     */
    void changeOutputColorWindows(LogMessageType type, std::string && logColor = "");
#elif defined(MAZE_PLATFORM_LINUX) || defined(MAZE_PLATFORM_APPLE)
    /**
     \brief Implements the colorized output for posix platforms (Apple and Linux platforms)
     \param type Type to use to get color as color
     \param logColor Color to apply to the message in case of custom log message type
    */
    void changeOutputColorPosix(LogMessageType type, std::string && logColor = "");
#endif
    
public:
    static Logs     Global;                 //> Default global Logs object that can be accessed from anywhere. The object is not default-initialized and must be initialized outside of this class
    
private:
    const bool      m_allowColorizedOutput; //> Defines if the messages in the output stream should be colorized
    std::mutex      m_mutex;                //> Mutex to lock the output stream access to prevent concurrent displays
    std::ostream&   m_outputStream;         //> Stream to use to output logs messages
};

/**
 \class Logs
 Usage exemple :
 \code
 // initialize Global Logs in main.cpp to make logs be displayed with colors in standart error stream
 mz::Logs mz::Logs::Global(std::cerr, true);
 
 // Display somme logs
 mz::Logs::Global.display("[description of the error]", mz::LogMessageType::ERROR);
 mz::Logs::Global.display("[A log message that will be displayed in cyan]", mz::LogMessageType::CUSTOM, MAZE_LOGS_GET_FILE_LOCATION, MAZE_LOG_COLOR_MAGENTA);
 \endcode
 */

}

#endif /* Logs_h */
