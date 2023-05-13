#ifndef LOGGER_H
#define LOGGER_H

#include <string>

class Logger {
    public: 
    enum LogType {
        SUCCESS, FAIL, MESSAGE, CALL
    };

    static void Log(std::string message, Logger::LogType logType);
};

#endif // LOGGER_H