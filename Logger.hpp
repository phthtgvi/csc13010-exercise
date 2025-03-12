#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include <string>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <iostream>

class Logger {
public:
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    void log(const std::string& message);

private:
    Logger() {}
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
};

#endif // LOGGER_HPP_

