#include "StatusRulesManager.hpp"

StatusRulesManager& StatusRulesManager::getInstance() {
    static StatusRulesManager instance;
    return instance;
}

StatusRulesManager::StatusRulesManager() : rulesFilename("status_rules.json") {
    loadRules(rulesFilename);
}

void StatusRulesManager::loadRules(const std::string& filename) {
    rulesFilename = filename;
    std::ifstream file(filename);
    if (file.is_open()) {
        try {
            json j;
            file >> j;
            rules.clear();
            for (auto it = j.begin(); it != j.end(); ++it) {
                std::string currentStatus = it.key();
                std::vector<std::string> allowed = it.value().get<std::vector<std::string>>();
                rules[currentStatus] = allowed;
            }
        } catch (const json::exception& e) {
            std::cerr << "Lỗi khi parse status rules: " << e.what() << std::endl;
        }
        file.close();
    } else {
        std::cout << "Không tìm thấy file quy luật, sử dụng cấu hình mặc định.\n";
        // Default configuration:
        rules["Active"] = {"Leave", "Graduated", "Absent"};
        rules["Leave"] = {"Active", "Graduated", "Absent"};
        rules["Absent"] = {"Active", "Leave"};
        rules["Graduated"] = {"Post-graduated"}; // Not allowed to transition back to "Active"
        rules["Post-graduated"] = {}; // No transitions allowed
        saveRules();
    }
}

void StatusRulesManager::saveRules() {
    json j;
    for (const auto& pair : rules) {
        j[pair.first] = pair.second;
    }
    std::ofstream file(rulesFilename);
    if (file.is_open()) {
        file << std::setw(4) << j << std::endl;
        file.close();
        std::cout << "Lưu quy luật thành công vào file: " << rulesFilename << "\n";
    } else {
        std::cerr << "Lỗi khi mở file để lưu quy luật: " << rulesFilename << std::endl;
    }
}

bool StatusRulesManager::isValidTransition(const std::string& oldStatus, const std::string& newStatus) {
    if (rules.find(oldStatus) == rules.end()) {
        // Allow transition if no rules defined for the old status
        return true;
    }
    const auto& allowed = rules[oldStatus];
    for (const auto& status : allowed) {
        if (status == newStatus) return true;
    }
    return false;
}

void StatusRulesManager::setAllowedTransitions(const std::string& currentStatus, const std::vector<std::string>& allowedTransitions) {
    rules[currentStatus] = allowedTransitions;
}

std::vector<std::string> StatusRulesManager::getAllowedTransitions(const std::string& currentStatus) {
    if (rules.find(currentStatus) != rules.end()) {
        return rules[currentStatus];
    }
    return {};
}
