#ifndef CERTIFICATE_GENERATOR_HPP_
#define CERTIFICATE_GENERATOR_HPP_

#include <string>

// Cấu trúc chứa thông tin cần thiết để tạo giấy xác nhận
struct CertificateData {
    // Thông tin trường
    std::string schoolName;
    std::string schoolAddress;
    std::string schoolPhone;
    std::string schoolEmail;

    // Thông tin sinh viên
    std::string studentName;
    std::string studentID;
    std::string studentDOB;       // Định dạng DD/MM/YYYY
    std::string studentGender;    // Male, Female, hoặc Khác
    std::string studentFaculty;
    std::string studentProgram;
    std::string studentCourse;    // Ví dụ: "K20 - 2020"
    std::string studentStatus;

    // Các thông tin khác
    std::string confirmationPurpose; // Nếu mục đích là "Xác nhận lý do khác"
    std::string effectiveDate;       // Giấy xác nhận có hiệu lực đến ngày (DD/MM/YYYY)
    std::string issueDate;           // Ngày cấp giấy (DD/MM/YYYY)
};

// Enum để chọn định dạng file xuất
enum class CertificateFormat {
    PDF,
    DOCX
};

// Hàm tạo giấy xác nhận với định dạng được chọn
bool generateCertificate(const CertificateData &data, const std::string &outputFile, CertificateFormat format);

#endif // CERTIFICATE_GENERATOR_HPP_
