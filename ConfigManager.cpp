#include "ConfigManager.hpp"

ConfigManager::ConfigManager() : configFilename("config.json") {
    loadConfig(configFilename);
}

// Load cấu hình từ file (mặc định "config.json")
void ConfigManager::loadConfig(const std::string& filename) {
    configFilename = filename;
    std::ifstream file(filename);
    if (file.is_open()) {
        try {
            json j;
            file >> j;
            emailSuffix = j.value("emailSuffix", emailSuffix);
            phoneRegex = j.value("phoneRegex", phoneRegex);
            deleteTimeLimit_ = j.value("deleteTimeLimit", deleteTimeLimit_);
            enforceValidation_ = j.value("enforceValidation", enforceValidation_);
        } catch (const json::exception& e) {
            std::cerr << "Lỗi khi parse config: " << e.what() << std::endl;
        }
        file.close();
    } else {
        std::cout << "Không tìm thấy file config. Sử dụng cấu hình mặc định.\n";
    }
}

// Lưu cấu hình vào file
void ConfigManager::saveConfig() {
    json j;
    j["emailSuffix"] = emailSuffix;
    j["phoneRegex"] = phoneRegex;
    j["deleteTimeLimit"] = deleteTimeLimit_;
    j["enforceValidation"] = enforceValidation_;
    std::ofstream file(configFilename);
    if (file.is_open()) {
        file << std::setw(4) << j << std::endl;
        file.close();
        std::cout << "Lưu cấu hình thành công vào file: " << configFilename << "\n";
    } else {
        std::cerr << "Lỗi mở file config để ghi: " << configFilename << std::endl;
    }
}

