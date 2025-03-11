#ifndef UNIT_TEST_HPP_
#define UNIT_TEST_HPP_

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
        config.setEmailSuffix("@student.university.edu.vn");
        config.setPhoneRegex("+84");
        config.saveConfig();

        // Reset lại để kiểm tra load
        config.setEmailSuffix("");
        config.setPhoneRegex("");
        config.loadConfig();

        assert(config.getEmailSuffix() == "@student.university.edu.vn");
        assert(config.getPhoneRegex() == "+84");
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

#endif // UNIT_TEST_HPP_


