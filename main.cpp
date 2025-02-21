#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <regex>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>

#include "nlohmann/json.hpp"
#include "Logger.hpp"
#include "RecordIO.hpp"

using json = nlohmann::json;

// Forward declaration
class Student;

// Interface cho việc xác thực dữ liệu sinh viên
class StudentValidator {
public:
    virtual bool isValid(const Student& student) = 0;
    virtual ~StudentValidator() = default;
};

// Lớp cơ sở cho Sinh viên
class Student {
public:
    // Constructor
    Student(std::string id, std::string name, std::string dob, std::string gender,
            std::string faculty, std::string course, std::string program, std::string address,
            std::string email, std::string phone, std::string status) :
        id_(id), name_(name), dob_(dob), gender_(gender), faculty_(faculty),
        course_(course), program_(program), address_(address), email_(email),
        phone_(phone), status_(status) {}

    // Phương thức ảo để hiển thị thông tin sinh viên (cho mục đích kế thừa)
    virtual void displayInfo() const {
        std::cout << "MSSV: " << id_ << "\n";
        std::cout << "Họ tên: " << name_ << "\n";
        std::cout << "Ngày sinh: " << dob_ << "\n";
        std::cout << "Giới tính: " << gender_ << "\n";
        std::cout << "Khoa: " << faculty_ << "\n";
        std::cout << "Khóa: " << course_ << "\n";
        std::cout << "Chương trình: " << program_ << "\n";
        std::cout << "Địa chỉ: " << address_ << "\n";
        std::cout << "Email: " << email_ << "\n";
        std::cout << "Số điện thoại: " << phone_ << "\n";
        std::cout << "Tình trạng: " << status_ << "\n";
    }

    // Getter methods
    std::string getId() const { return id_; }
    std::string getName() const { return name_; }
    std::string getDob() const { return dob_; }
    std::string getAddress() const { return address_; }
    std::string getGender() const { return gender_; }
    std::string getCourse() const { return course_; }
    std::string getProgram() const { return program_; }
    std::string getEmail() const { return email_; }
    std::string getPhone() const { return phone_; }
    std::string getStatus() const { return status_; }
    std::string getFaculty() const { return faculty_; }

    // Setter methods
    void setName(const std::string& name) { name_ = name; }
    void setDob(const std::string& dob) { dob_ = dob; }
    void setGender(const std::string& gender) { gender_ = gender; }
    void setFaculty(const std::string& faculty) { faculty_ = faculty; }
    void setCourse(const std::string& course) { course_ = course; }
    void setProgram(const std::string& program) { program_ = program; }
    void setAddress(const std::string& address) { address_ = address; }
    void setEmail(const std::string& email) { email_ = email; }
    void setPhone(const std::string& phone) { phone_ = phone; }
    void setStatus(const std::string& status) { status_ = status; }

    // Method to serialize Student object to JSON
    json toJson() const {
        return {
            {"id", id_},
            {"name", name_},
            {"dob", dob_},
            {"gender", gender_},
            {"faculty", faculty_},
            {"course", course_},
            {"program", program_},
            {"address", address_},
            {"email", email_},
            {"phone", phone_},
            {"status", status_}
        };
    }

    // Static method to create Student object from JSON
    static Student fromJson(const json& j) {
        return Student(
            j["id"].get<std::string>(),
            j["name"].get<std::string>(),
            j["dob"].get<std::string>(),
            j["gender"].get<std::string>(),
            j["faculty"].get<std::string>(),
            j["course"].get<std::string>(),
            j["program"].get<std::string>(),
            j["address"].get<std::string>(),
            j["email"].get<std::string>(),
            j["phone"].get<std::string>(),
            j["status"].get<std::string>()
        );
    }

private:
    std::string id_;
    std::string name_;
    std::string dob_;
    std::string gender_;
    std::string faculty_;
    std::string course_;
    std::string program_;
    std::string address_;
    std::string email_;
    std::string phone_;
    std::string status_;
};

// Lớp xác thực dữ liệu sinh viên
class ConcreteStudentValidator : public StudentValidator {
public:
    bool isValid(const Student& student) override {
        if (!isValidEmail(student.getEmail())) {
            std::cout << "Email không hợp lệ.\n";
            return false;
        }
        if (!isValidPhone(student.getPhone())) {
            std::cout << "Số điện thoại không hợp lệ.\n";
            return false;
        }
         if (!isValidFaculty(student.getFaculty())) {
            std::cout << "Khoa không hợp lệ.\n";
            return false;
        }
        if (!isValidStatus(student.getStatus())) {
            std::cout << "Tình trạng sinh viên không hợp lệ.\n";
            return false;
        }
        if (!isValidGender(student.getGender())) {
            std::cout << "Giới tính không hợp lệ. (Male, Female)\n";
            return false;
        }
        if (!isValidProgram(student.getProgram())) {
            std::cout << "Chương trình không hợp lệ. (Advanced Program, Formal Program, High Quality Program)\n";
            return false;
        }
        if (!isValidCourse(student.getCourse())) {
            std::cout << "Khóa không hợp lệ. (YYYY)\n";
            return false;
        }
        if (!isValidDOB(student.getDob())) {
            std::cout << "Ngày sinh không hợp lệ. (DD/MM/YYYY)\n";
            return false;
        }
        return true;
    }

private:
    bool isValidEmail(const std::string& email) {
        const std::regex pattern(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
        return std::regex_match(email, pattern);
    }

    bool isValidPhone(const std::string& phone) {
        const std::regex pattern(R"(\d{10})");
        return std::regex_match(phone, pattern);
    }

    bool isValidFaculty(const std::string& faculty) {
        return (faculty == "FL" || faculty == "FBE" || faculty == "FJPN" || faculty == "FFR");
    }

    bool isValidStatus(const std::string& status) {
        return (status == "Active" || status == "Graduated" || status == "Leave" || status == "Absent");
    }

    bool isValidGender(const std::string& gender) {
        return (gender == "Male" || gender == "Female");
    }

    bool isValidProgram(const std::string& program) {
        return (program == "Advanced Program" || program == "Formal Program" || program == "High Quality Program");
    }

    bool isValidCourse(const std::string& course) {
        const std::regex pattern(R"(\d{4})");
        return std::regex_match(course, pattern);
    }

    bool isValidDOB(const std::string& dob) {
        const std::regex pattern(R"(\d{2}/\d{2}/\d{4})");
        return std::regex_match(dob, pattern);
    }
};

class StudentRepository {
public:
    static StudentRepository& getInstance() {
        static StudentRepository instance;
        return instance;
    }

    void addStudent(const Student& student) {
        if (validator_->isValid(student)) {
            students_.push_back(student);
            saveDataToFile();
            displayAllStudents();
            std::cout << "Đã thêm sinh viên thành công.\n";
            Logger::getInstance().log("Added student with ID: " + student.getId()); // Log the action
        }
        else {
            std::cout << "Không thể thêm sinh viên do thông tin không hợp lệ.\n";
            Logger::getInstance().log("Failed to add student due to invalid information."); // Log the action
        }
    }

    void removeStudent(const std::string& id) {
        auto it = std::remove_if(students_.begin(), students_.end(),
            [&](const Student& s) { return s.getId() == id; });

        if (it != students_.end()) {
            students_.erase(it, students_.end());
            saveDataToFile();
            displayAllStudents();
            std::cout << "Đã xóa sinh viên thành công.\n";
            Logger::getInstance().log("Removed student with ID: " + id); // Log the action
        }
        else {
            std::cout << "Không tìm thấy sinh viên với ID này.\n";
            Logger::getInstance().log("Attempted to remove student with ID: " + id + " but not found."); // Log the action
        }
    }

    Student* findStudent(const std::string& id) {
        for (Student& student : students_) {
            if (student.getId() == id) {
                return &student;
            }
        }
        return nullptr;
    }

    std::vector<Student> searchStudents(const std::string& keyword) {
        std::vector<Student> results;
        for (const Student& student : students_) {
            if (student.getName().find(keyword) != std::string::npos || student.getId().find(keyword) != std::string::npos) {
                results.push_back(student);
            }
        }
        Logger::getInstance().log("Searched students with keyword: " + keyword);
        return results;
    }

    void setValidator(StudentValidator* validator) {
        delete validator_;
        validator_ = validator;
    }

    // Method to get all students as a vector of vectors of strings
    std::vector<std::vector<std::string>> getAllStudentsAsStrings() const {
        std::vector<std::vector<std::string>> studentStrings;
        for (const auto& student : students_) {
            std::vector<std::string> studentData;
            studentData.push_back(student.getId());
            studentData.push_back(student.getName());
            studentData.push_back(student.getDob());
            studentData.push_back(student.getGender());
            studentData.push_back(student.getFaculty());
            studentData.push_back(student.getCourse());
            studentData.push_back(student.getProgram());
            studentData.push_back(student.getAddress());
            studentData.push_back(student.getEmail());
            studentData.push_back(student.getPhone());
            studentData.push_back(student.getStatus());
            studentStrings.push_back(studentData);
        }
        return studentStrings;
    }

    // Method to import students from a vector of vectors of strings
    void importStudentsFromStrings(const std::vector<std::vector<std::string>>& studentStrings) {
        bool flag = true;
        for (const auto& studentData : studentStrings) {
            if (studentData.size() == 11) {
                Student newStudent(
                    studentData[0],  // id
                    studentData[1],  // name
                    studentData[2],  // dob
                    studentData[3],  // gender
                    studentData[4],  // faculty
                    studentData[5],  // course
                    studentData[6],  // program
                    studentData[7],  // address
                    studentData[8],  // email
                    studentData[9],  // phone
                    studentData[10] // status
                );

                if (validator_->isValid(newStudent)) {
                    students_.push_back(newStudent);
                } else {
                    flag = false;
                    std::cout << "Thông tin sinh viên không hợp lệ: " << studentData[0] << std::endl;
                }
            } else {
                std::cout << "Dữ liệu không hợp lệ, bỏ qua sinh viên." << std::endl;
            }
        }
        if (flag == true) std::cout << "Nhập dữ liệu thành công" << std::endl;
        saveDataToFile();
    }

    ~StudentRepository() {
        delete validator_;
    }

    void loadDataFromFile() {
        std::ifstream file(filename_);
        if (file.is_open()) {
            json j;
            file >> j;
            for (auto& item : j) {
                students_.push_back(Student::fromJson(item));
            }
            file.close();
            Logger::getInstance().log("Loaded student data from file.");
        }
        else {
            std::cout << "Không thể mở file để đọc dữ liệu. Tạo file mới.\n";
            Logger::getInstance().log("Could not open file to load data. Creating new file.");
        }
    }

    void saveDataToFile() {
        json j = json::array();
        for (const auto& student : students_) {
            j.push_back(student.toJson());
        }
        std::ofstream file(filename_);
        file << std::setw(4) << j << std::endl;
        file.close();
        Logger::getInstance().log("Saved student data to file.");
    }

    void displayAllStudents() {
        std::cout << "\n--- Danh sách sinh viên ---" << std::endl;
        if (students_.empty()) {
            std::cout << "Danh sách trống.\n";
            return;
        }
        for (const Student& student : students_) {
            student.displayInfo();
            std::cout << "----------\n";
        }
    }
private:
    StudentRepository() : validator_(new ConcreteStudentValidator()) {
        loadDataFromFile();
    }
    StudentRepository(const StudentRepository&) = delete;
    StudentRepository& operator=(const StudentRepository&) = delete;

    std::vector<Student> students_;
    StudentValidator* validator_;
    const std::string filename_ = "students.json";

};

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

    std::cout << "Nhập khoa (FL, FBE, FJPN, FFR): ";
    std::getline(std::cin >> std::ws, faculty);

    std::cout << "Nhập khóa (YYYY): ";
    std::getline(std::cin >> std::ws, course);

    std::cout << "Nhập chương trình (Advanced Program, Formal Program, High Quality Program): ";
    std::getline(std::cin >> std::ws, program);

    std::cout << "Nhập địa chỉ: ";
    std::getline(std::cin >> std::ws, address);

    std::cout << "Nhập email: ";
    std::getline(std::cin >> std::ws, email);

    std::cout << "Nhập số điện thoại: ";
    std::getline(std::cin >> std::ws, phone);

    std::cout << "Nhập tình trạng (Active, Graduated, Leave, Absent): ";
    std::getline(std::cin >> std::ws, status);

    return Student(id, name, dob, gender, faculty, course, program, address, email, phone, status);
}

bool getUpdatedStudentInfoFromUser(Student* student, ConcreteStudentValidator* validator) {
    std::string name, dob, gender, faculty, course, program, address, email, phone, status;
    std::cout << "Nhập họ tên (" << student->getName() << "): ";
    std::getline(std::cin, name);
    if (!name.empty()) student->setName(name);

    std::cout << "Nhập ngày tháng năm sinh (DD/MM/YYYY) (" << student->getDob() << "): ";
    std::getline(std::cin, dob);
    if (!dob.empty()) student->setDob(dob);

    std::cout << "Nhập giới tính (Male, Female) (" << student->getGender() << "): ";
    std::getline(std::cin, gender);
    if (!gender.empty()) student->setGender(gender);

    std::cout << "Nhập khoa (FL, FBE, FJPN, FFR) (" << student->getFaculty() << "): ";
    std::getline(std::cin, faculty);
    if (!faculty.empty()) student->setFaculty(faculty);

    std::cout << "Nhập khóa (YYYY) (" << student->getCourse() << "): ";
    std::getline(std::cin, course);
    if (!course.empty()) student->setCourse(course);

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

    std::cout << "Nhập tình trạng (Active, Graduated, Leave, Absent) (" << student->getStatus() << "): ";
    std::getline(std::cin, status);
    if (!status.empty()) student->setStatus(status);

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

    std::cout << "Application Version: " << version << std::endl;
    std::cout << "Build Date: " << buildDate << std::endl;
    std::cout << "-------------------------" << std::endl;

    StudentRepository& repo = StudentRepository::getInstance();
    ConcreteStudentValidator* validator = new ConcreteStudentValidator();
    RecordIO recordIO;

    int choice;
    do {
        repo.displayAllStudents();
        std::cout << "\n--- MENU ---" << std::endl;
        std::cout << "1. Thêm sinh viên" << std::endl;
        std::cout << "2. Xóa sinh viên" << std::endl;
        std::cout << "3. Cập nhật thông tin sinh viên" << std::endl;
        std::cout << "4. Tìm kiếm sinh viên" << std::endl;
        std::cout << "5. Nhập dữ liệu từ CSV" << std::endl;
        std::cout << "6. Xuất dữ liệu ra CSV" << std::endl;
        std::cout << "7. Nhập dữ liệu từ JSON" << std::endl;
        std::cout << "8. Xuất dữ liệu ra JSON" << std::endl;
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
                        repo.saveDataToFile();
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
