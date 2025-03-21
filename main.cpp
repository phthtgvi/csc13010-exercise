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
#include <cstdio>

#include "nlohmann/json.hpp"
#include "Logger.hpp"
#include "RecordIO.hpp"
#include "Student.hpp"
#include "ConfigManager.hpp"
#include "StatusRulesManager.hpp"
#include "UnitTest.hpp"
#include "CertificateGenerator.hpp"

using json = nlohmann::json;

// Hàm dịch trạng thái sinh viên từ tiếng Anh sang tiếng Việt
std::string translateStatus(const std::string &status) {
    if (status == "Active") {
        return "Đang theo học";
    } else if (status == "Graduated") {
        return "Đã tốt nghiệp";
    } else if (status == "Leave") {
        return "Bảo lưu";
    } else if (status == "Absent") {
        return "Vắng mặt";
    } else if (status == "Post-graduated") {
        return "Sau tốt nghiệp";
    } else {
        return status; // Nếu không khớp, trả về giá trị ban đầu
    }
}

// Hàm hỗ trợ người dùng chọn mục đích xác nhận
std::string chooseCertificatePurpose() {
    std::cout << "\nChọn mục đích xác nhận:\n";
    std::cout << "1. Xác nhận đang học để vay vốn ngân hàng\n";
    std::cout << "2. Xác nhận làm thủ tục tạm hoãn nghĩa vụ quân sự\n";
    std::cout << "3. Xác nhận làm hồ sơ xin việc / thực tập\n";
    std::cout << "4. Xác nhận lý do khác\n";
    std::cout << "Nhập lựa chọn (1-4): ";

    int choice;
    std::cin >> choice;
    std::cin.ignore(); // xóa ký tự newline

    std::string purpose;
    switch (choice) {
        case 1:
            purpose = "Xác nhận đang học để vay vốn ngân hàng";
            break;
        case 2:
            purpose = "Xác nhận làm thủ tục tạm hoãn nghĩa vụ quân sự";
            break;
        case 3:
            purpose = "Xác nhận làm hồ sơ xin việc / thực tập";
            break;
        case 4:
        {
            std::cout << "Nhập lý do khác: ";
            std::getline(std::cin, purpose);
            purpose = " Xác nhận lý do khác - " + purpose;
            break;
        }
        default:
            std::cout << "Lựa chọn không hợp lệ. Mặc định: Xác nhận đang học để vay vốn ngân hàng.\n";
            purpose = "Xác nhận đang học để vay vốn ngân hàng";
            break;
    }
    return purpose;
}

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

int main() {
    auto [version, buildDate] = getVersionInfo("version_info.json");
    std::cout << "-------------------------" << std::endl;
    std::cout << "Ho Chi Minh City University of Science" << std::endl;
    std::cout << "Student Management System " << version << std::endl;
    std::cout << "Application Version: " << version << std::endl;
    std::cout << "Build Date: " << buildDate << std::endl;
    std::cout << "-------------------------" << std::endl;
    std::cout << "Email Suffix: " << ConfigManager::getInstance().getEmailSuffix() << std::endl;
    std::cout << "Phone Regex: " << ConfigManager::getInstance().getPhoneRegex() << std::endl;
    std::cout << "Delete time limit window: " << ConfigManager::getInstance().getDeleteTimeLimit() << std::endl;
    std::cout << "-------------------------" << std::endl;
    ConfigManager::getInstance().loadConfig();
    StatusRulesManager::getInstance().loadRules();

    StudentRepository& repo = StudentRepository::getInstance();
    ConcreteStudentValidator* validator = new ConcreteStudentValidator(&repo);
    repo.setValidator(validator);
    RecordIO recordIO;

    #ifdef UNIT_TEST
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
    #endif

    int choice;
    do {
        repo.loadStudentDataFromFile();
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
        std::cout << "15. Xuất giấy xác nhận tình trạng sinh viên" << std::endl;
        std::cout << "16. Cấu hình thời gian xóa sinh viên" << std::endl;
        std::cout << "17. Xóa Khoa" << std::endl;
        std::cout << "18. Xóa Tình trạng" << std::endl;
        std::cout << "19. Xóa Chương trình đào tạo" << std::endl;
        std::cout << "20. Bật/Tắt quy định" << std::endl;
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
            case 15: {
                std::string studentID;
                std::cout << "Nhập MSSV: ";
                std::getline(std::cin, studentID);

                // Mở file JSON (database) chứa dữ liệu sinh viên
                std::ifstream inFile("students.json");
                if (!inFile.is_open()) {
                    std::cerr << "Error: Không thể mở file database (students.json).\n";
                    return 1;
                }

                json db;
                try {
                    inFile >> db;
                } catch (const json::exception &e) {
                    std::cerr << "Error parsing JSON: " << e.what() << "\n";
                    return 1;
                }
                inFile.close();

                // Tìm sinh viên theo MSSV
                bool found = false;
                CertificateData cert;
                for (const auto &record : db) {
                    if (record.contains("id") && record["id"].get<std::string>() == studentID) {
                        found = true;
                        // Lấy thông tin sinh viên từ record JSON
                        cert.studentID       = record["id"].get<std::string>();
                        cert.studentName     = record["name"].get<std::string>();
                        cert.studentDOB      = record["dob"].get<std::string>();
                        cert.studentGender   = record["gender"].get<std::string>();
                        cert.studentFaculty  = record["faculty"].get<std::string>();
                        cert.studentProgram  = record["program"].get<std::string>();
                        cert.studentCourse   = record["course"].get<std::string>();
                        // Nếu record có trường "status", chuyển đổi sang tiếng Việt
                        if (record.contains("status"))
                            cert.studentStatus = translateStatus(record["status"].get<std::string>());
                        else
                            cert.studentStatus = "Không xác định";

                        // Thông tin trường (có thể thay đổi theo cấu hình)
                        cert.schoolName      = "KHOA HỌC TỰ NHIÊN";
                        cert.schoolAddress   = "227 NGUYỄN VĂN CỪ, PHƯỜNG 4, QUẬN 5 TP.HCM";
                        cert.schoolPhone     = "(028) 38353448";
                        cert.schoolEmail     = "contact@hcmus.edu.vn";

                        // Lấy mục đích xác nhận thông qua hàm giao diện
                        cert.confirmationPurpose = chooseCertificatePurpose();
                        std::cout << "Nhập ngày hiệu lực (DD/MM/YYYY): ";
                        std::getline(std::cin, cert.effectiveDate);
                        std::cout << "Nhập ngày cấp (DD/MM/YYYY): ";
                        std::getline(std::cin, cert.issueDate);
                        break;
                    }
                }

                if (!found) {
                    std::cerr << "Không tìm thấy sinh viên có MSSV: " << studentID << "\n";
                    return 1;
                }

                // Hỏi người dùng chọn định dạng xuất giấy xác nhận
                int formatChoice = 0;
                std::cout << "\nChọn định dạng xuất giấy xác nhận:\n";
                std::cout << "1. Markdown (certificate.md)\n";
                std::cout << "2. DOCX (certificate.docx)\n";
                std::cout << "Nhập lựa chọn của bạn: ";
                std::cin >> formatChoice;
                std::cin.ignore(); // xóa newline

                CertificateFormat format;
                std::string outputFile;
                if (formatChoice == 1) {
                    format = CertificateFormat::MD;
                    outputFile = "outputs/certificate.md";
                } else if (formatChoice == 2) {
                    format = CertificateFormat::DOCX;
                    outputFile = "outputs/certificate.docx";
                } else {
                    std::cerr << "Lựa chọn không hợp lệ.\n";
                    return 1;
                }

                // Tạo giấy xác nhận
                if (generateCertificate(cert, outputFile, format)) {
                    std::cout << "Giấy xác nhận đã được tạo thành công: " << outputFile << "\n";
                } else {
                    std::cerr << "Tạo giấy xác nhận thất bại.\n";
                }
                break;
            }
            case 16: { // Cấu hình thời gian xóa sinh viên
                std::string thresholdStr = repo.getSafeInput("Nhập thời gian (phút) cho phép xóa sinh viên kể từ thời điểm tạo: ");
                try {
                    int threshold = std::stoi(thresholdStr);
                    ConfigManager::getInstance().setDeleteTimeLimit(threshold);
                    ConfigManager::getInstance().saveConfig();
                    std::cout << "Cấu hình thời gian xóa đã được cập nhật thành: " << threshold << " phút.\n";
                } catch (std::exception &e) {
                    std::cout << "Lỗi nhập số: " << e.what() << "\n";
                }
                break;
            }

            case 17: { // Xóa khoa
                std::string faculty = repo.getSafeInput("Nhập tên khoa cần xóa: ");
                if (repo.deleteFaculty(faculty)) {
                    std::cout << "Xóa khoa thành công.\n";
                } else {
                    std::cout << "Xóa khoa thất bại. Có thể khoa này đang được sử dụng bởi sinh viên nào đó.\n";
                }
                break;
            }
            case 18: { // Xóa tình trạng sinh viên
                std::string status = repo.getSafeInput("Nhập tên tình trạng cần xóa: ");
                if (repo.deleteStatus(status)) {
                    std::cout << "Xóa tình trạng thành công.\n";
                } else {
                    std::cout << "Xóa tình trạng thất bại. Có thể tình trạng này đang được sử dụng bởi sinh viên nào đó.\n";
                }
                break;
            }
            case 19: { // Xóa chương trình đào tạo
                std::string program = repo.getSafeInput("Nhập tên chương trình cần xóa: ");
                if (repo.deleteProgram(program)) {
                    std::cout << "Xóa chương trình đào tạo thành công.\n";
                } else {
                    std::cout << "Xóa chương trình đào tạo thất bại. Có thể chương trình này đang được sử dụng bởi sinh viên nào đó.\n";
                }
                break;
            }
            case 20: { // Bật/Tắt cả quy định: định dạng email & số điện thoại và thời gian xóa sinh viên
                std::string choice;
                // Hiển thị trạng thái hiện tại dựa vào việc cả hai cờ đều bật hay không
                bool currentState = ConfigManager::getInstance().getEnforceValidation();
                std::cout << "Hiện tại, các quy định đang " << (currentState ? "BẬT" : "TẮT") << ".\n";
                std::cout << "Nhập 'on' để bật, 'off' để tắt tất cả hoặc nhập 'exit' để thoát: ";
                std::getline(std::cin, choice);
                bool flag;
                if (choice == "exit" || choice == "EXIT") {
                    break;
                }
                if (choice == "on" || choice == "ON") {
                    flag = true;
                } else if (choice == "off" || choice == "OFF") {
                    flag = false;
                } else {
                    std::cout << "Lựa chọn không hợp lệ.\n";
                    break;
                }
                ConfigManager::getInstance().setEnforceValidation(flag);
                ConfigManager::getInstance().saveConfig();
                std::cout << "Các quy định hiện tại: " << (flag ? "BẬT" : "TẮT") << ".\n";
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
