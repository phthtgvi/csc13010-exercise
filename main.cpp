#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <regex>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <cassert>
#include <iostream>
#include <cstdio>

#include "nlohmann/json.hpp"
#include "Logger.hpp"
#include "RecordIO.hpp"
#include "Student.hpp"
#include "ConfigManager.hpp"
#include "StatusRulesManager.hpp"


using json = nlohmann::json;

Student getStudentInfoFromUser() {
    std::string id, name, dob, gender, faculty, course, program, address, email, phone, status;

    std::cout << "Nhập MSSV: ";
    std::getline(std::cin >> std::ws, id);

    std::cout << "Nhập họ tên: ";
    std::getline(std::cin >> std::ws, name);

    std::cout << "Nhập ngày tháng năm sinh (DD/MM/YYYY): ";
    std::getline(std::cin >> std::ws, dob);

    std::cout << "Nhập giới tính (Male, Female): ";
    std::getline(std::cin >> std::ws, gender);


    StudentRepository& repo = StudentRepository::getInstance();
    repo.displayFaculties();
    std::cout << "Nhập khoa (FL, FBE, FJPN, FFR): ";
    std::getline(std::cin >> std::ws, faculty);

    std::cout << "Nhập khóa (YYYY): ";
    std::getline(std::cin >> std::ws, course);

    repo.displayPrograms();
    std::cout << "Nhập chương trình (Advanced Program, Formal Program, High Quality Program): ";
    std::getline(std::cin >> std::ws, program);

    std::cout << "Nhập địa chỉ: ";
    std::getline(std::cin >> std::ws, address);

    std::cout << "Nhập email: ";
    std::getline(std::cin >> std::ws, email);

    std::cout << "Nhập số điện thoại: ";
    std::getline(std::cin >> std::ws, phone);

    repo.displayStatuses();
    std::cout << "Nhập tình trạng (Active, Graduated, Leave, Absent): ";
    std::getline(std::cin >> std::ws, status);

    return Student(id, name, dob, gender, faculty, course, program, address, email, phone, status);
}

bool getUpdatedStudentInfoFromUser(Student* student, ConcreteStudentValidator* validator) {
    std::string id, name, dob, gender, faculty, course, program, address, email, phone, status;

    // Lấy repository để kiểm tra MSSV
    StudentRepository& repo = StudentRepository::getInstance();

    std::cout << "Nhập MSSV (" << student->getId() << "): ";
    std::getline(std::cin, id);

    // Nếu người dùng nhập MSSV mới, kiểm tra xem nó có trùng không
    if (!id.empty() && id != student->getId()) {
        if (repo.isStudentIdExists(id)) {
            std::cout << "Lỗi: MSSV " << id << " đã tồn tại!\n";
            Logger::getInstance().log("Failed to update student - ID already exists: " + id);
            return false;
        }
        student->setId(id);
    }

    std::cout << "Nhập họ tên (" << student->getName() << "): ";
    std::getline(std::cin, name);
    if (!name.empty()) student->setName(name);

    std::cout << "Nhập ngày tháng năm sinh (DD/MM/YYYY) (" << student->getDob() << "): ";
    std::getline(std::cin, dob);
    if (!dob.empty()) student->setDob(dob);

    std::cout << "Nhập giới tính (Male, Female) (" << student->getGender() << "): ";
    std::getline(std::cin, gender);
    if (!gender.empty()) student->setGender(gender);

    repo.displayFaculties();
    std::cout << "Nhập khoa: ";
    std::getline(std::cin, faculty);
    if (!faculty.empty()) student->setFaculty(faculty);

    std::cout << "Nhập khóa (YYYY) (" << student->getCourse() << "): ";
    std::getline(std::cin, course);
    if (!course.empty()) student->setCourse(course);

    repo.displayPrograms();
    std::cout << "Nhập chương trình (Advanced Program, Formal Program, High Quality Program) (" << student->getProgram() << "): ";
    std::getline(std::cin, program);
    if (!program.empty()) student->setProgram(program);

    std::cout << "Nhập địa chỉ (" << student->getAddress() << "): ";
    std::getline(std::cin, address);
    if (!address.empty()) student->setAddress(address);

    std::cout << "Nhập email (" << student->getEmail() << "): ";
    std::getline(std::cin, email);
    if (!email.empty()) student->setEmail(email);

    std::cout << "Nhập số điện thoại (" << student->getPhone() << "): ";
    std::getline(std::cin, phone);
    if (!phone.empty()) student->setPhone(phone);

    repo.displayStatuses();
    std::cout << "Nhập tình trạng (ví dụ: Active, Graduated, Leave, Absent) (" << student->getStatus() << "): ";
    std::getline(std::cin, status);
    if (!status.empty()) {
        // Kiểm tra quy luật chuyển đổi: trạng thái cũ -> trạng thái mới
        std::string oldStatus = student->getStatus();
        if (!StatusRulesManager::getInstance().isValidTransition(oldStatus, status)) {
            std::cout << "Chuyển đổi từ trạng thái \"" << oldStatus << "\" sang \"" << status << "\" không hợp lệ.\n";
            Logger::getInstance().log("Failed to update student with ID: " + student->getId() + " due to invalid status transition from " + oldStatus + " to " + status);
            return false;
        }
        student->setStatus(status);
    }

    // Sau đó, kiểm tra hợp lệ tổng thể
    if (!validator->isValid(*student)) {
        std::cout << "Thông tin sinh viên không hợp lệ. Cập nhật bị hủy bỏ.\n";
        Logger::getInstance().log("Failed to update student with ID: " + student->getId() + " due to invalid information.");
        return false;
    }

    Logger::getInstance().log("Updated student with ID: " + student->getId());
    return true;
}

std::pair<std::string, std::string> getVersionInfo(const std::string& filepath) {
    std::ifstream file(filepath);
    std::string version = "N/A";
    std::string buildDate = "N/A";

    if (file.is_open()) {
        try {
            json j;
            file >> j;
            version = j.value("version", "N/A");
            buildDate = j.value("builtDate", "N/A");
        } catch (const json::exception& e) {
            std::cerr << "Error parsing version info from JSON file: " << e.what() << std::endl;
        }
        file.close();
    } else {
        std::cerr << "Could not open version info file: " << filepath << std::endl;
    }

    return {version, buildDate};
}


namespace Test {
    // Hàm hỗ trợ xóa sạch danh sách sinh viên (nếu cần)
    void clearStudentRepository() {
        StudentRepository& repo = StudentRepository::getInstance();
        auto allStudents = repo.getAllStudentsAsStrings();
        for (auto & studentData : allStudents) {
            std::string id = studentData[0];
            repo.removeStudent(id);
        }
    }

    // Test: Sinh viên chuyển sang JSON rồi deserialize về và so sánh các trường
    void testStudentSerialization() {
        Student s("SV001", "Alice", "01/01/2000", "Female", "FL", "2020",
                  "Advanced Program", "Address 1", "alice@student.university.edu.vn",
                  "0123456789", "Active");
        json j = s.toJson();
        Student s2 = Student::fromJson(j);

        assert(s.getId() == s2.getId());
        assert(s.getName() == s2.getName());
        assert(s.getDob() == s2.getDob());
        assert(s.getGender() == s2.getGender());
        assert(s.getFaculty() == s2.getFaculty());
        assert(s.getCourse() == s2.getCourse());
        assert(s.getProgram() == s2.getProgram());
        assert(s.getAddress() == s2.getAddress());
        assert(s.getEmail() == s2.getEmail());
        assert(s.getPhone() == s2.getPhone());
        assert(s.getStatus() == s2.getStatus());

        std::cout << "testStudentSerialization passed.\n";
    }

    // Test: Thêm, kiểm tra trùng lặp và xóa sinh viên trong StudentRepository
    void testStudentRepository() {
        clearStudentRepository();
        StudentRepository& repo = StudentRepository::getInstance();

        Student s("SV001", "Alice", "01/01/2000", "Female", "Faculty of Law", "2020",
                  "Advanced Program", "Address 1", "alice@student.university.edu.vn",
                  "+84123456789", "Active");

        repo.addStudent(s);
        Student* found = repo.findStudent("SV001");
        assert(found != nullptr);
        assert(found->getName() == "Alice");

        // Thử thêm lại sinh viên có cùng MSSV (sẽ không thêm)
        size_t countBefore = repo.getAllStudentsAsStrings().size();
        repo.addStudent(s);
        size_t countAfter = repo.getAllStudentsAsStrings().size();
        assert(countAfter == countBefore);

        // Xóa sinh viên
        repo.removeStudent("SV001");
        Student* foundAfterRemove = repo.findStudent("SV001");
        assert(foundAfterRemove == nullptr);

        std::cout << "testStudentRepository passed.\n";
    }

    // Test: Xuất và nhập file CSV
    void testRecordIO_CSV() {
        RecordIO recordIO;
        std::string filename = "test_export.csv";
        std::vector<std::vector<std::string>> records = {
            {"SV001", "Alice", "01/01/2000", "Female", "FL", "2020", "Advanced Program", "Address 1", "alice@student.university.edu.vn", "0123456789", "Active"},
            {"SV002", "Bob", "02/02/2000", "Male", "FBE", "2020", "Formal Program", "Address 2", "bob@student.university.edu.vn", "0987654321", "Graduated"}
        };

        bool exportResult = recordIO.exportToCSV(filename, records);
        assert(exportResult == true);

        auto importedRecords = recordIO.importFromCSV(filename);
        assert(importedRecords.size() == records.size());
        assert(importedRecords[0] == records[0]);

        std::remove(filename.c_str());
        std::cout << "testRecordIO_CSV passed.\n";
    }

    // Test: Xuất và nhập file JSON
    void testRecordIO_JSON() {
        RecordIO recordIO;
        std::string filename = "test_export.json";
        std::vector<std::vector<std::string>> records = {
            {"SV001", "Alice", "01/01/2000", "Female", "FL", "2020", "Advanced Program", "Address 1", "alice@student.university.edu.vn", "0123456789", "Active"},
            {"SV002", "Bob", "02/02/2000", "Male", "FBE", "2020", "Formal Program", "Address 2", "bob@student.university.edu.vn", "0987654321", "Graduated"}
        };

        bool exportResult = recordIO.exportToJSON(filename, records);
        assert(exportResult == true);

        auto importedRecords = recordIO.importFromJSON(filename);
        assert(importedRecords.size() == records.size());
        assert(importedRecords[0] == records[0]);

        std::remove(filename.c_str());
        std::cout << "testRecordIO_JSON passed.\n";
    }

    // Test: Cấu hình email và phone thông qua ConfigManager
    void testConfigManager() {
        ConfigManager& config = ConfigManager::getInstance();
        config.setEmailSuffix("@student.hcmus.edu.vn");
        config.setPhoneRegex("+35");
        config.saveConfig();

        // Reset lại để kiểm tra load
        config.setEmailSuffix("");
        config.setPhoneRegex("");
        config.loadConfig();

        assert(config.getEmailSuffix() == "@student.hcmus.edu.vn");
        assert(config.getPhoneRegex() == "+35");
        std::cout << "testConfigManager passed.\n";
    }

    // Test: Quy luật chuyển đổi trạng thái với StatusRulesManager
    void testStatusRulesManager() {
        StatusRulesManager& srm = StatusRulesManager::getInstance();
        srm.loadRules(); // Sử dụng cấu hình mặc định

        // Kiểm tra chuyển từ Active sang Leave (hợp lệ)
        assert(srm.isValidTransition("Active", "Leave") == true);
        // Kiểm tra chuyển từ Graduated sang Active (không hợp lệ)
        assert(srm.isValidTransition("Graduated", "Active") == false);
        // Kiểm tra chuyển từ Post-graduated về bất kỳ trạng thái nào (không hợp lệ)
        assert(srm.isValidTransition("Post-graduated", "Active") == false);

        std::cout << "testStatusRulesManager passed.\n";
    }

    // Test: Kiểm tra email và số điện thoại qua ConcreteStudentValidator
    void testConcreteStudentValidator() {
        // Đặt cấu hình hợp lệ cho email và phone
        ConfigManager::getInstance().setEmailSuffix("@student.university.edu.vn");
        ConfigManager::getInstance().setPhoneRegex("+84");

        // Reset repository
        StudentRepository& repo = StudentRepository::getInstance();
        clearStudentRepository();

        ConcreteStudentValidator validator(&repo);

        // Sinh viên hợp lệ
        Student s_valid("SV001", "Alice", "01/01/2000", "Female", "Faculty of Law", "2020",
                        "Advanced Program", "Address 1", "alice@student.university.edu.vn",
                        "+8431234567", "Active");
        assert(validator.isValid(s_valid) == true);

        // Sinh viên với email không hợp lệ
        Student s_invalid_email("SV002", "Bob", "02/02/2000", "Male", "FBE", "2020",
                                "Formal Program", "Address 2", "bob@otherdomain.com",
                                "0123456789", "Active");
        assert(validator.isValid(s_invalid_email) == false);

        // Sinh viên với phone không hợp lệ
        Student s_invalid_phone("SV003", "Charlie", "03/03/2000", "Male", "FBE", "2020",
                                "Formal Program", "Address 3", "charlie@student.university.edu.vn",
                                "123456", "Active");
        assert(validator.isValid(s_invalid_phone) == false);

        std::cout << "testConcreteStudentValidator passed.\n";
    }
}

int main() {
    auto [version, buildDate] = getVersionInfo("version_info.json");

    std::cout << "Application Version: " << version << std::endl;
    std::cout << "Build Date: " << buildDate << std::endl;
    std::cout << "-------------------------" << std::endl;

    ConfigManager::getInstance().loadConfig();
    StatusRulesManager::getInstance().loadRules();

    StudentRepository& repo = StudentRepository::getInstance();
    ConcreteStudentValidator* validator = new ConcreteStudentValidator(&repo);
    repo.setValidator(validator);
    RecordIO recordIO;

  try {
        Test::testStudentSerialization();
        Test::testStudentRepository();
        Test::testRecordIO_CSV();
        Test::testRecordIO_JSON();
        Test::testConfigManager();
        Test::testStatusRulesManager();
        Test::testConcreteStudentValidator();

        std::cout << "Tất cả unit test đã chạy thành công.\n";
    } catch (const std::exception& ex) {
        std::cerr << "Unit test thất bại: " << ex.what() << "\n";
        return 1;
    } catch (...) {
        std::cerr << "Unit test thất bại với lỗi không xác định.\n";
        return 1;
    }

    int choice;
    do {
        repo.displayAllStudents();
        std::cout << "\n--- MENU ---" << std::endl;
        std::cout << "1. Thêm sinh viên" << std::endl;
        std::cout << "2. Xóa sinh viên" << std::endl;
        std::cout << "3. Cập nhật thông tin sinh viên" << std::endl;
        std::cout << "4. Tìm kiếm sinh viên (theo MSSV hoặc Họ tên)" << std::endl;
        std::cout << "5. Nhập dữ liệu từ CSV" << std::endl;
        std::cout << "6. Xuất dữ liệu ra CSV" << std::endl;
        std::cout << "7. Nhập dữ liệu từ JSON" << std::endl;
        std::cout << "8. Xuất dữ liệu ra JSON" << std::endl;
        std::cout << "9. Tìm kiếm sinh viên (theo Khoa hoặc Khoa và Họ tên)" << std::endl;
        std::cout << "10. Quản lý Khoa" << std::endl;
        std::cout << "11. Quản lý Tình trạng" << std::endl;
        std::cout << "12. Quản lý Chương trình" << std::endl;
        std::cout << "13. Cấu hình định dạng Email & Số điện thoại" << std::endl;
        std::cout << "14. Cấu hình quy luật chuyển đổi Status" << std::endl;

        std::cout << "0. Thoát" << std::endl;
        std::cout << "Nhập lựa chọn của bạn: ";
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1: {
                Student newStudent = getStudentInfoFromUser();
                repo.addStudent(newStudent);
                break;
            }
            case 2: {
                std::string id;
                std::cout << "Nhập MSSV của sinh viên cần xóa: ";
                std::getline(std::cin, id);
                repo.removeStudent(id);
                break;
            }

            case 3: {
                std::string id;
                std::cout << "Nhập MSSV của sinh viên cần cập nhật: ";
                std::getline(std::cin, id);

                Student* student = repo.findStudent(id);
                if (student) {
                    if (getUpdatedStudentInfoFromUser(student, validator)) {
                        repo.saveStudentDataToFile();
                        repo.displayAllStudents();
                        std::cout << "Thông tin sinh viên đã được cập nhật.\n";
                    } else {
                        std::cout << "Cập nhật thông tin sinh viên bị hủy bỏ do không hợp lệ.\n";
                    }
                } else {
                    std::cout << "Không tìm thấy sinh viên với MSSV này.\n";
                }
                break;
            }

            case 4: {
                std::string keyword;
                std::cout << "Nhập từ khóa tìm kiếm (Họ tên hoặc MSSV): ";
                std::getline(std::cin, keyword);
                std::vector<Student> results = repo.searchStudents(keyword);
                if (!results.empty()) {
                    std::cout << "Kết quả tìm kiếm:\n";
                    for (const Student& student : results) {
                        student.displayInfo();
                        std::cout << "----------\n";
                    }
                } else {
                    std::cout << "Không tìm thấy sinh viên nào phù hợp.\n";
                }
                break;
            }

            case 5: {
                std::string filename;
                std::cout << "Nhập tên file CSV để nhập: ";
                std::getline(std::cin, filename);
                std::vector<std::vector<std::string>> importedData = recordIO.importFromCSV(filename);
                repo.importStudentsFromStrings(importedData);
                break;
            }

            case 6: {
                std::string filename;
                std::cout << "Nhập tên file CSV để xuất: ";
                std::getline(std::cin, filename);
                std::vector<std::vector<std::string>> allStudents = repo.getAllStudentsAsStrings();
                recordIO.exportToCSV(filename, allStudents);
                break;
            }

            case 7: {
                std::string filename;
                std::cout << "Nhập tên file JSON để nhập: ";
                std::getline(std::cin, filename);
                std::vector<std::vector<std::string>> importedData = recordIO.importFromJSON(filename);
                repo.importStudentsFromStrings(importedData);
                break;
            }
            case 8: {
                std::string filename;
                std::cout << "Nhập tên file JSON để xuất: ";
                std::getline(std::cin, filename);
                std::vector<std::vector<std::string>> allStudents = repo.getAllStudentsAsStrings();
                recordIO.exportToJSON(filename, allStudents);
                break;
            }
            case 9: {
                std::string faculty, name;
                std::cout << "Nhập Khoa cần tìm kiếm ('Họ tên' để trống nếu chỉ tìm theo Khoa): ";
                std::getline(std::cin, faculty);
                std::getline(std::cin, name);

                std::vector<Student> results = repo.searchStudents(faculty, name);
                if (!results.empty()) {
                    std::cout << "Kết quả tìm kiếm:\n";
                    for (const auto& s : results) {
                        s.displayInfo();
                        std::cout << "----------\n";
                    }
                } else {
                    std::cout << "Không tìm thấy sinh viên nào phù hợp.\n";
                }
                break;
            }
           case 10: { // Faculty Management
                int facultyChoice;
                std::cout << "\n--- Quản lý Khoa ---" << std::endl;
                std::cout << "1. Thêm Khoa" << std::endl;
                std::cout << "2. Đổi tên Khoa" << std::endl;
                std::cout << "Nhập lựa chọn của bạn: ";
                std::cin >> facultyChoice;
                std::cin.ignore();

                if (facultyChoice == 1) {
                    std::string newFaculty = repo.getSafeInput("Nhập tên Khoa mới: ");
                    repo.addFaculty(newFaculty);
                } else if (facultyChoice == 2) {
                    std::string oldFaculty = repo.getSafeInput("Nhập tên Khoa cần đổi: ");
                    std::string newFaculty = repo.getSafeInput("Nhập tên Khoa mới: ");
                    repo.renameFaculty(oldFaculty, newFaculty);
                } else {
                    std::cout << "Lựa chọn không hợp lệ.\n";
                }
                break;
            }
            case 11: { // Status Management
                int statusChoice;
                std::cout << "\n--- Quản lý Tình trạng ---" << std::endl;
                std::cout << "1. Thêm Tình trạng" << std::endl;
                std::cout << "2. Đổi tên Tình trạng" << std::endl;
                std::cout << "Nhập lựa chọn của bạn: ";
                std::cin >> statusChoice;
                std::cin.ignore();

                if (statusChoice == 1) {
                    std::string newStatus = repo.getSafeInput("Nhập tên Tình trạng mới: ");
                    repo.addStatus(newStatus);
                } else if (statusChoice == 2) {
                    std::string oldStatus = repo.getSafeInput("Nhập tên Tình trạng cần đổi: ");
                    std::string newStatus = repo.getSafeInput("Nhập tên Tình trạng mới: ");
                    repo.renameStatus(oldStatus, newStatus);
                } else {
                    std::cout << "Lựa chọn không hợp lệ.\n";
                }
                break;
            }
            case 12: { // Program Management
                int programChoice;
                std::cout << "\n--- Quản lý Chương trình ---" << std::endl;
                std::cout << "1. Thêm Chương trình" << std::endl;
                std::cout << "2. Đổi tên Chương trình" << std::endl;
                std::cout << "Nhập lựa chọn của bạn: ";
                std::cin >> programChoice;
                std::cin.ignore();

                if (programChoice == 1) {
                    std::string newProgram = repo.getSafeInput("Nhập tên Chương trình mới: ");
                    repo.addProgram(newProgram);
                } else if (programChoice == 2) {
                    std::string oldProgram = repo.getSafeInput("Nhập tên Chương trình cần đổi: ");
                    std::string newProgram = repo.getSafeInput("Nhập tên Chương trình mới: ");
                    repo.renameProgram(oldProgram, newProgram);
                } else {
                    std::cout << "Lựa chọn không hợp lệ.\n";
                }
                break;
            }
            case 13: { // Cấu hình Email và Số điện thoại
                std::string newEmailSuffix = repo.getSafeInput("Nhập đuôi email mới (ví dụ: @student.university.edu.vn): ");
                std::string newPhoneRegex = repo.getSafeInput("Nhập biểu thức chính quy cho số điện thoại (ví dụ: +84: ");

                ConfigManager::getInstance().setEmailSuffix(newEmailSuffix);
                ConfigManager::getInstance().setPhoneRegex(newPhoneRegex);
                ConfigManager::getInstance().saveConfig();
                std::cout << "Cấu hình đã được cập nhật.\n";
                break;
            }
            case 14: { // Cấu hình quy luật chuyển đổi Status
                std::string currentStatus = repo.getSafeInput("Nhập trạng thái hiện tại cần cấu hình: ");
                std::string allowedStr = repo.getSafeInput("Nhập các trạng thái cho phép chuyển đổi (cách nhau bởi dấu phẩy): ");

                // Tách chuỗi allowedStr thành vector<string>
                std::vector<std::string> allowed;
                std::istringstream ss(allowedStr);
                std::string token;
                while (std::getline(ss, token, ',')) {
                    // Loại bỏ khoảng trắng ở đầu và cuối
                    token.erase(0, token.find_first_not_of(" \t"));
                    token.erase(token.find_last_not_of(" \t") + 1);
                    if (!token.empty())
                        allowed.push_back(token);
                }

                StatusRulesManager::getInstance().setAllowedTransitions(currentStatus, allowed);
                StatusRulesManager::getInstance().saveRules();
                break;
            }
            case 0:
                std::cout << "Thoát chương trình.\n";
                break;
            default:
                std::cout << "Lựa chọn không hợp lệ. Vui lòng thử lại.\n";
        }
    } while (choice != 0);
    delete validator;
    return 0;
}
