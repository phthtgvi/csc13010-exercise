#include "Logger.hpp"

std::string getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time_now = std::chrono::system_clock::to_time_t(now);
    std::tm local_tm;
    localtime_r(&time_now, &local_tm); // Use localtime_r on Linux
    std::stringstream ss;
    ss << std::put_time(&local_tm, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

 void Logger::log(const std::string& message) {
    std::ofstream logFile;
    logFile.open("student_management.log", std::ios::app);
    if (logFile.is_open()) {
        logFile << getCurrentTimestamp() << ": " << message << std::endl;
        logFile.close();
    }
    else {
        std::cerr << "Error opening log file.\n";
    }
}


