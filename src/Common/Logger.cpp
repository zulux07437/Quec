#include "Logger.h"
#include <iostream>

void Logger::Log(std::string message, Logger::LogType logType) {
    std::string type = "";
    switch (logType) {
        case Logger::LogType::SUCCESS: type = "[SUCCESS] ";    break;
        case Logger::LogType::FAIL:    type = "[FAIL] ";       break;
        case Logger::LogType::MESSAGE: type = "[MESSAGE] ";    break;
        case Logger::LogType::CALL:    type = "[CALL] ";       break;
        default:                       type = "[UNKNOWN(?)] "; break;
    }
    std::cout << type << message << std::endl;
}