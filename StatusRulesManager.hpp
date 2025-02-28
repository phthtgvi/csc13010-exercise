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
    static StatusRulesManager& getInstance() {
        static StatusRulesManager instance;
        return instance;
    }

    // Load quy luật từ file JSON (mặc định "status_rules.json")
    void loadRules(const std::string& filename = "status_rules.json") {
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
            // Cấu hình mặc định sử dụng các trạng thái:
            // "Active", "Graduated", "Leave", "Absent", "Post-graduated"
            rules["Active"] = {"Leave", "Graduated", "Absent"};
            rules["Leave"] = {"Active", "Graduated", "Absent"};
            rules["Absent"] = {"Active", "Leave"};
            rules["Graduated"] = {"Post-graduated"}; // Không cho phép chuyển về "Active"
            rules["Post-graduated"] = {}; // Không cho phép thay đổi
            saveRules();
        }
    }

    // Lưu quy luật vào file JSON
    void saveRules() {
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

    // Kiểm tra chuyển đổi từ trạng thái cũ sang trạng thái mới có hợp lệ không
    bool isValidTransition(const std::string& oldStatus, const std::string& newStatus) {
        if (rules.find(oldStatus) == rules.end()) {
            // Nếu không có quy luật nào được định nghĩa cho trạng thái cũ, ta cho phép chuyển đổi
            return true;
        }
        const auto& allowed = rules[oldStatus];
        for (const auto& status : allowed) {
            if (status == newStatus) return true;
        }
        return false;
    }

    // Thiết lập lại quy luật cho một trạng thái hiện tại
    void setAllowedTransitions(const std::string& currentStatus, const std::vector<std::string>& allowedTransitions) {
        rules[currentStatus] = allowedTransitions;
    }

    // Lấy danh sách trạng thái cho phép chuyển đổi từ trạng thái hiện tại
    std::vector<std::string> getAllowedTransitions(const std::string& currentStatus) {
        if (rules.find(currentStatus) != rules.end()) {
            return rules[currentStatus];
        }
        return {};
    }

private:
    StatusRulesManager() : rulesFilename("status_rules.json") {
        loadRules(rulesFilename);
    }
    StatusRulesManager(const StatusRulesManager&) = delete;
    StatusRulesManager& operator=(const StatusRulesManager&) = delete;

    std::string rulesFilename;
    std::unordered_map<std::string, std::vector<std::string>> rules;
};

#endif // STATUS_RULES_MANAGER_HPP_
