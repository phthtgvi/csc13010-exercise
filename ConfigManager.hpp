
#ifndef CONFIG_MANAGER_HPP_
#define CONFIG_MANAGER_HPP_

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

class ConfigManager {
public:
    static ConfigManager& getInstance() {
        static ConfigManager instance;
        return instance;
    }

    // Load cấu hình từ file (mặc định "config.json")
    void loadConfig(const std::string& filename = "config.json") {
        configFilename = filename;
        std::ifstream file(filename);
        if (file.is_open()) {
            try {
                json j;
                file >> j;
                emailSuffix = j.value("emailSuffix", emailSuffix);
                phoneRegex = j.value("phoneRegex", phoneRegex);
            } catch (const json::exception& e) {
                std::cerr << "Lỗi khi parse config: " << e.what() << std::endl;
            }
            file.close();
        } else {
            std::cout << "Không tìm thấy file config. Sử dụng cấu hình mặc định.\n";
        }
    }

    // Lưu cấu hình vào file
    void saveConfig() {
        json j;
        j["emailSuffix"] = emailSuffix;
        j["phoneRegex"] = phoneRegex;
        std::ofstream file(configFilename);
        if (file.is_open()) {
            file << std::setw(4) << j << std::endl;
            file.close();
            std::cout << "Lưu cấu hình thành công vào file: " << configFilename << "\n";
        } else {
            std::cerr << "Lỗi mở file config để ghi: " << configFilename << std::endl;
        }
    }

    // Setters & Getters
    void setEmailSuffix(const std::string& suffix) { emailSuffix = suffix; }
    void setPhoneRegex(const std::string& regex) { phoneRegex = regex; }

    std::string getEmailSuffix() const { return emailSuffix; }
    std::string getPhoneRegex() const { return phoneRegex; }

private:
    ConfigManager() : configFilename("config.json") {
        loadConfig(configFilename);
    }

    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;

    std::string emailSuffix;
    std::string phoneRegex;
    std::string configFilename;
};

#endif // CONFIG_MANAGER_HPP_
