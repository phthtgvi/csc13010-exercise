#ifndef STATUS_RULES_MANAGER_HPP_
#define STATUS_RULES_MANAGER_HPP_

#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

class StatusRulesManager {
public:
    // Singleton instance getter
    static StatusRulesManager& getInstance();

    // Load rules from a JSON file (default: "status_rules.json")
    void loadRules(const std::string& filename = "status_rules.json");

    // Save rules to a JSON file
    void saveRules();

    // Check whether transitioning from oldStatus to newStatus is allowed
    bool isValidTransition(const std::string& oldStatus, const std::string& newStatus);

    // Set allowed transitions for a given current status
    void setAllowedTransitions(const std::string& currentStatus, const std::vector<std::string>& allowedTransitions);

    // Get allowed transitions for a given current status
    std::vector<std::string> getAllowedTransitions(const std::string& currentStatus);

private:
    // Private constructor for singleton pattern
    StatusRulesManager();
    // Delete copy constructor and assignment operator
    StatusRulesManager(const StatusRulesManager&) = delete;
    StatusRulesManager& operator=(const StatusRulesManager&) = delete;

    std::string rulesFilename;
    std::unordered_map<std::string, std::vector<std::string>> rules;
};

#endif // STATUS_RULES_MANAGER_HPP_
