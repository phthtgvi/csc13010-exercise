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

    void setDeleteTimeLimit(int minutes) { deleteTimeLimit_ = minutes; }
    int getDeleteTimeLimit() const { return deleteTimeLimit_; }

    void loadConfig(const std::string& filename = "config.json");
    void saveConfig();

    static ConfigManager& getInstance() {
        static ConfigManager instance;
        return instance;
    }

    // Setters & Getters
    void setEmailSuffix(const std::string& suffix) { emailSuffix = suffix; }
    void setPhoneRegex(const std::string& regex) { phoneRegex = regex; }
    void setEnforceValidation(bool flag) { enforceValidation_ = flag; }
    bool getEnforceValidation() const { return enforceValidation_; }
    std::string getEmailSuffix() const { return emailSuffix; }
    std::string getPhoneRegex() const { return phoneRegex; }

private:
    ConfigManager();
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;

    int deleteTimeLimit_ = 1;
    std::string emailSuffix;
    std::string phoneRegex;
    std::string configFilename;
    bool enforceValidation_ = true;
};

#endif // CONFIG_MANAGER_HPP_
