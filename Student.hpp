#ifndef STUDENT_HPP_
#define STUDENT_HPP_

#include "ConfigManager.hpp"
#include "Logger.hpp"

// Forward declaration
class Student;

std::string timePointToISO8601(const std::chrono::system_clock::time_point& tp) {
    std::time_t time = std::chrono::system_clock::to_time_t(tp);
    std::tm tm = *std::gmtime(&time);
    std::stringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%dT%H:%M:%SZ");
    return ss.str();
}

// Chuyển chuỗi ISO 8601 (UTC) thành time_point
std::chrono::system_clock::time_point iso8601ToTimePoint(const std::string &s) {
    std::tm tm = {};
    std::istringstream iss(s);
    iss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%SZ");
    // Sử dụng timegm để chuyển std::tm theo UTC thành time_t
    std::time_t time = timegm(&tm);
    return std::chrono::system_clock::from_time_t(time);
}

// Lớp cơ sở cho Sinh viên
class Student {
public:
    // Constructor
    Student(std::string id, std::string name, std::string dob, std::string gender,
            std::string faculty, std::string course, std::string program, std::string address,
            std::string email, std::string phone, std::string status)
      : id_(id), name_(name), dob_(dob), gender_(gender), faculty_(faculty),
        course_(course), program_(program), address_(address), email_(email),
        phone_(phone), status_(status), creationTime_(std::chrono::system_clock::now()) {}

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
    std::chrono::system_clock::time_point getCreationTime() const { return creationTime_; }

    // Setter methods
    void setId(const std::string& id) { id_ = id; }
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
    void setCreationTime(const std::chrono::system_clock::time_point& t) { creationTime_ = t; }

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
            {"status", status_},
            {"creationTime", timePointToISO8601(creationTime_)},
        };
    }

    // Static method to create Student object from JSON
    static Student fromJson(const json& j) {
        // Tạo đối tượng sinh viên ban đầu (không truyền creationTime vào constructor)
        Student s(
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
        // Cập nhật trường creationTime từ chuỗi ISO 8601
        s.setCreationTime(iso8601ToTimePoint(j["creationTime"].get<std::string>()));
        return s;
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
    std::chrono::system_clock::time_point creationTime_;
};

// Interface cho việc xác thực dữ liệu sinh viên
class StudentValidator {
public:
    virtual bool isValid(const Student& student) = 0;
    virtual ~StudentValidator() = default;
};


class StudentRepository {
public:
    static StudentRepository& getInstance() {
        static StudentRepository instance;
        return instance;
    }

    bool isStudentIdExists(const std::string& id) const {
    return std::any_of(students_.begin(), students_.end(),
                       [&](const Student& student) { return student.getId() == id; });
    }

    std::string getSafeInput(const std::string& prompt) {
        std::string input;
        std::cout << prompt;
        std::getline(std::cin >> std::ws, input);  // Read with leading whitespace skipped
        return input;
    }

    bool deleteFaculty(const std::string &faculty) {
    // Kiểm tra xem có sinh viên nào thuộc khoa này không
        for (const auto &student : students_) {
            if (student.getFaculty() == faculty) {
                std::cout << "Không thể xóa khoa '" << faculty << "' vì có sinh viên được gán vào khoa này.\n";
                return false;
            }
        }
        // Nếu không có sinh viên nào thuộc khoa này, tiến hành xóa khỏi danh sách khoa
        auto it = std::remove(faculties_.begin(), faculties_.end(), faculty);
        if (it != faculties_.end()) {
            faculties_.erase(it, faculties_.end());
            saveDataToFile(facultyFilename_, faculties_);
            std::cout << "Đã xóa khoa: " << faculty << "\n";
            return true;
        }
        std::cout << "Không tìm thấy khoa '" << faculty << "'.\n";
        return false;
    }

    // Xóa tình trạng nếu không có sinh viên nào sử dụng tình trạng đó.
    bool deleteStatus(const std::string &status) {
        for (const auto &student : students_) {
            if (student.getStatus() == status) {
                std::cout << "Không thể xóa tình trạng '" << status << "' vì có sinh viên được gán vào tình trạng này.\n";
                return false;
            }
        }
        auto it = std::remove(statuses_.begin(), statuses_.end(), status);
        if (it != statuses_.end()) {
            statuses_.erase(it, statuses_.end());
            saveDataToFile(statusFilename_, statuses_);
            std::cout << "Đã xóa tình trạng: " << status << "\n";
            return true;
        }
        std::cout << "Không tìm thấy tình trạng '" << status << "'.\n";
        return false;
    }

    // Xóa chương trình đào tạo nếu không có sinh viên nào được gán vào chương trình đó.
    bool deleteProgram(const std::string &program) {
        for (const auto &student : students_) {
            if (student.getProgram() == program) {
                std::cout << "Không thể xóa chương trình '" << program << "' vì có sinh viên được gán vào chương trình này.\n";
                return false;
            }
        }
        auto it = std::remove(programs_.begin(), programs_.end(), program);
        if (it != programs_.end()) {
            programs_.erase(it, programs_.end());
            saveDataToFile(programFilename_, programs_);
            std::cout << "Đã xóa chương trình: " << program << "\n";
            return true;
        }
        std::cout << "Không tìm thấy chương trình '" << program << "'.\n";
        return false;
    }

    void addStudent(const Student& student) {
        // Kiểm tra xem MSSV đã tồn tại hay chưa
        for (const auto& existingStudent : students_) {
            if (existingStudent.getId() == student.getId()) {
                std::cout << "Lỗi: MSSV " << student.getId() << " đã tồn tại!\n";
                Logger::getInstance().log("Failed to add student - ID already exists: " + student.getId());
                return;
            }
        }

        if (validator_ == nullptr) {
            std::cerr << "Validator chưa được thiết lập!\n";
            return;
        }

        if (validator_->isValid(student)) {
            // Tạo bản sao của sinh viên để cập nhật thời gian tạo
            Student newStudent = student;
            newStudent.setCreationTime(std::chrono::system_clock::now()); // Cập nhật thời gian tạo

            students_.push_back(newStudent);
            saveStudentDataToFile();
            std::cout << "Đã thêm sinh viên thành công.\n";
            Logger::getInstance().log("Added student with ID: " + student.getId());
        } else {
            std::cout << "Không thể thêm sinh viên do thông tin không hợp lệ.\n";
            Logger::getInstance().log("Failed to add student due to invalid information.");
        }
    }


    bool removeStudent(const std::string& id) {
        auto it = std::find_if(students_.begin(), students_.end(),
                               [&](const Student& s) { return s.getId() == id; });
        if (it != students_.end()) {
            // Lấy thời gian hiện tại
            auto now = std::chrono::system_clock::now();
            auto diff = std::chrono::duration_cast<std::chrono::minutes>(now - it->getCreationTime());
            int allowed = ConfigManager::getInstance().getDeleteTimeLimit();
            if (diff.count() > allowed) {
                std::cout << "Không được phép xóa sinh viên sau " << allowed << " phút kể từ thời điểm tạo.\n";
                return false;
            }
            // Nếu hợp lệ, xóa sinh viên
            students_.erase(it);
            saveStudentDataToFile();
            std::cout << "Đã xóa sinh viên thành công.\n";
            Logger::getInstance().log("Removed student with ID: " + id);
            return true;
        } else {
            std::cout << "Không tìm thấy sinh viên với MSSV này.\n";
            return false;
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

    std::vector<Student> searchStudents(const std::string& faculty, const std::string& name = "") {
        std::vector<Student> results;
        for (const Student& student : students_) {
            if (student.getFaculty() == faculty) {
                if (name.empty() || (student.getName().find(name) != std::string::npos)) {
                    results.push_back(student);
                }
            }
        }
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
        saveStudentDataToFile();
    }

    ~StudentRepository() {
        delete validator_;
    }

    void loadStudentDataFromFile() {
        std::ifstream file(studentFilename_);
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

    void saveStudentDataToFile() {
        json j = json::array();
        for (const auto& student : students_) {
            j.push_back(student.toJson());
        }
        std::ofstream file(studentFilename_);
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

    //-----------------------------------------------------------------------
    // Faculty Management
    //-----------------------------------------------------------------------

    void addFaculty(const std::string& faculty) {
        if (!isValidFaculty(faculty)) {
            faculties_.push_back(faculty);
            std::cout << "Đã thêm khoa mới: " << faculty << ".\n";
            saveDataToFile(facultyFilename_, faculties_);
        } else {
            std::cout << "Khoa này đã tồn tại.\n";
        }
    }

    void renameFaculty(const std::string& oldFaculty, const std::string& newFaculty) {
        if (isValidFaculty(newFaculty)) {
            std::cout << "Tên khoa mới đã tồn tại.\n";
            return;
        }

        bool found = false;
        for (auto& student : students_) {
            if (student.getFaculty() == oldFaculty) {
                student.setFaculty(newFaculty);
                found = true;
            }
        }

        if (found) {
            // Update the faculty list
            std::replace(faculties_.begin(), faculties_.end(), oldFaculty, newFaculty);
            std::cout << "Đã đổi tên khoa " << oldFaculty << " thành " << newFaculty << ".\n";

            saveStudentDataToFile(); // Update student data because faculty has been changed
            saveDataToFile(facultyFilename_, faculties_);
        } else {
            std::cout << "Không tìm thấy khoa " << oldFaculty << ".\n";
        }
    }

    bool isValidFaculty(const std::string& faculty) const {
        return std::find(faculties_.begin(), faculties_.end(), faculty) != faculties_.end();
    }

    void displayFaculties() const {
        std::cout << "\n--- Danh sách các Khoa ---" << std::endl;
        for (const auto& faculty : faculties_) {
            std::cout << faculty << std::endl;
        }
    }

    //-----------------------------------------------------------------------
    // Status Management
    //-----------------------------------------------------------------------
    void addStatus(const std::string& status) {
        if (!isValidStatus(status)) {
            statuses_.push_back(status);
            std::cout << "Đã thêm tình trạng mới: " << status << ".\n";
            saveDataToFile(statusFilename_, statuses_);
        } else {
            std::cout << "Tình trạng này đã tồn tại.\n";
        }
    }

    void renameStatus(const std::string& oldStatus, const std::string& newStatus) {
        if (isValidStatus(newStatus)) {
            std::cout << "Tên tình trạng mới đã tồn tại.\n";
            return;
        }

        bool found = false;
        for (auto& student : students_) {
            if (student.getStatus() == oldStatus) {
                student.setStatus(newStatus);
                found = true;
            }
        }

        if (found) {
            // Update the status list
            std::replace(statuses_.begin(), statuses_.end(), oldStatus, newStatus);
            std::cout << "Đã đổi tên tình trạng " << oldStatus << " thành " << newStatus << ".\n";
            saveStudentDataToFile(); // Update student data because status has been changed
            saveDataToFile(statusFilename_, statuses_);
        } else {
            std::cout << "Không tìm thấy tình trạng " << oldStatus << ".\n";
        }
    }

    bool isValidStatus(const std::string& status) const {
        return std::find(statuses_.begin(), statuses_.end(), status) != statuses_.end();
    }

    void displayStatuses() const {
        std::cout << "\n--- Danh sách các Tình trạng ---" << std::endl;
        for (const auto& status : statuses_) {
            std::cout << status << std::endl;
        }
    }

    //-----------------------------------------------------------------------
    // Program Management
    //-----------------------------------------------------------------------
    void addProgram(const std::string& program) {
        if (!isValidProgram(program)) {
            programs_.push_back(program);
            std::cout << "Đã thêm chương trình mới: " << program << ".\n";
            saveDataToFile(programFilename_, programs_);
        } else {
            std::cout << "Chương trình này đã tồn tại.\n";
        }
    }

    void renameProgram(const std::string& oldProgram, const std::string& newProgram) {
        if (isValidProgram(newProgram)) {
            std::cout << "Tên chương trình mới đã tồn tại.\n";
            return;
        }

        bool found = false;
        for (auto& student : students_) {
            if (student.getProgram() == oldProgram) {
                student.setProgram(newProgram);
                found = true;
            }

        }

        if (found) {
            // Update the program list
            std::replace(programs_.begin(), programs_.end(), oldProgram, newProgram);
            std::cout << "Đã đổi tên chương trình " << oldProgram << " thành " << newProgram << ".\n";
            saveStudentDataToFile(); // Update student data because program has been changed
            saveDataToFile(programFilename_, programs_);        } else {
            std::cout << "Không tìm thấy chương trình " << oldProgram << ".\n";
        }
    }

    bool isValidProgram(const std::string& program) const {
        return std::find(programs_.begin(), programs_.end(), program) != programs_.end();
    }

    void displayPrograms() const {
        std::cout << "\n--- Danh sách các Chương trình ---" << std::endl;
        for (const auto& program : programs_) {
            std::cout << program << std::endl;
        }
    }

private:
    StudentRepository() : validator_(nullptr) {
        loadStudentDataFromFile();
        loadDataFromFile(facultyFilename_, faculties_);
        loadDataFromFile(statusFilename_, statuses_);
        loadDataFromFile(programFilename_, programs_);
    }
    StudentRepository(const StudentRepository&) = delete;
    StudentRepository& operator=(const StudentRepository&) = delete;

    void loadDataFromFile(const std::string& filename, std::vector<std::string>& data) {
        std::ifstream file(filename);
        if (file.is_open()) {
            json j;
            file >> j;
            data = j.get<std::vector<std::string>>();
        } else {
                std::cout << "Không thể mở file để đọc dữ liệu " << filename << ". Tạo file mới.\n";
        }
    }


    // Helper function to save data to file

    void saveDataToFile(const std::string& filename, const std::vector<std::string>& data) {
        json j = data;
        std::ofstream file(filename);
        file << std::setw(4) << j << std::endl;
        file.close();
    }

    std::vector<Student> students_;
    StudentValidator* validator_;
    const std::string studentFilename_ = "students.json";

    // Filenames for Faculty, Status, and Program

    const std::string facultyFilename_ = "faculties.json";
    const std::string statusFilename_ = "statuses.json";
    const std::string programFilename_ = "programs.json";

    // Data Members for Faculty, Status, and Program
    std::vector<std::string> faculties_ = {"FL", "FBE", "FJPN", "FFR"};  // Initial values
    std::vector<std::string> statuses_ = {"Active", "Graduated", "Leave", "Absent"}; // Initial values
    std::vector<std::string> programs_ = {"Advanced Program", "Formal Program", "High Quality Program"}; // Initial values
};


class ConcreteStudentValidator : public StudentValidator {
public:
    ConcreteStudentValidator(class StudentRepository* repo) : repo_(repo) {}

    bool isValid(const Student& student) override {
        if (!isValidEmail(student.getEmail())) {
            std::cout << "Email không hợp lệ.\n";
            return false;
        }
        if (!isValidPhone(student.getPhone())) {
            std::cout << "Số điện thoại không hợp lệ.\n";
            return false;
        }
        if (!repo_->isValidFaculty(student.getFaculty())) {
            std::cout << "Khoa không hợp lệ.\n";
            return false;
        }
        if (!repo_->isValidStatus(student.getStatus())) {
            std::cout << "Tình trạng sinh viên không hợp lệ.\n";
            return false;
        }
        if (!isValidGender(student.getGender())) {
            std::cout << "Giới tính không hợp lệ. (Male, Female)\n";
            return false;
        }
        if (!repo_->isValidProgram(student.getProgram())) {
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
    // Kiểm tra email: phải kết thúc với đuôi đã cấu hình
    bool isValidEmail(const std::string& email) {
        std::string suffix = ConfigManager::getInstance().getEmailSuffix();
        if (email.size() < suffix.size()) return false;
        return email.compare(email.size() - suffix.size(), suffix.size(), suffix) == 0;
    }

    std::string escapeRegex(const std::string& str) {
        std::string escaped;
        for (char c : str) {
            if (std::string(".^$|()[]{}*+?\\").find(c) != std::string::npos) {
                escaped.push_back('\\');
            }
            escaped.push_back(c);
        }
        return escaped;
    }

    bool isValidPhone(const std::string& phone) {
        // Lấy cấu hình phoneRegex từ ConfigManager
        std::string phonePattern = ConfigManager::getInstance().getPhoneRegex();
        std::cout << "Cấu hình phoneRegex: " << phonePattern << "\n";

        // Nếu chuỗi không bắt đầu bằng '^', cho rằng đây là chuỗi literal và escape nó
        if (phonePattern.empty() || phonePattern.front() != '^') {
            phonePattern = "^" + escapeRegex(phonePattern) + ".*$";
        }

        try {
            std::regex pattern(phonePattern);
            return std::regex_match(phone, pattern);
        } catch (std::regex_error& e) {
            std::cerr << "Regex error: " << e.what() << std::endl;
            return false;
        }
    }

    // Các hàm xác thực cũ (gender, course, DOB) giữ nguyên...
    bool isValidGender(const std::string& gender) {
        return (gender == "Male" || gender == "Female");
    }

    bool isValidCourse(const std::string& course) {
        const std::regex pattern(R"(\d{4})");
        return std::regex_match(course, pattern);
    }

    bool isValidDOB(const std::string& dob) {
        const std::regex pattern(R"(\d{2}/\d{2}/\d{4})");
        return std::regex_match(dob, pattern);
    }

    StudentRepository* repo_;
};


#endif // STUDENT_HPP_

