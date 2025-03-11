#include "CertificateGenerator.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

// Function to generate certificate in Markdown format
bool generateCertificateMarkdown(const CertificateData &data, const std::string &outputFile) {
    std::ofstream out(outputFile);
    if (!out.is_open()) {
        std::cerr << "Error: Cannot open output file for Markdown generation.\n";
        return false;
    }

    // Header with school and contact info
    out << "# TRƯỜNG ĐẠI HỌC " << data.schoolName << "\n";
    out << "## PHÒNG ĐÀO TẠO\n\n";
    out << "**📍 Địa chỉ:** " << data.schoolAddress << "  \n";
    out << "**📞 Điện thoại:** " << data.schoolPhone
        << " | **📧 Email:** " << data.schoolEmail << "\n\n";
    out << "---\n\n";

    // Title of the certificate
    out << "# GIẤY XÁC NHẬN TÌNH TRẠNG SINH VIÊN\n\n";
    out << "Trường Đại học " << data.schoolName << " xác nhận:\n\n";

    // Student Information
    out << "## 1. Thông tin sinh viên:\n";
    out << "- **Họ và tên:** " << data.studentName << "\n";
    out << "- **Mã số sinh viên:** " << data.studentID << "\n";
    out << "- **Ngày sinh:** " << data.studentDOB << "\n";
    out << "- **Giới tính:** " << data.studentGender << "\n";
    out << "- **Khoa:** " << data.studentFaculty << "\n";
    out << "- **Chương trình đào tạo:** " << data.studentProgram << "\n";
    out << "- **Khóa:** " << data.studentCourse << "\n";
    out << "- **Tình trạng:** " << data.studentStatus << "\n\n";

    // Purpose of certificate
    out << "## 3. Mục đích xác nhận:\n";
    out << "**Mục đích:** " << data.confirmationPurpose << "\n\n";

    // Certificate validity period
    out << "## 4. Thời gian cấp giấy:\n";
    out << "- Giấy xác nhận có hiệu lực đến ngày: " << data.effectiveDate << "\n\n";

    // Footer with issuing information
    out << "**Xác nhận của Trường Đại học " << data.schoolName << "**\n\n";
    out << "**Ngày cấp:** " << data.issueDate << "\n\n";
    out << "**Trưởng Phòng Đào Tạo**  \n";
    out << "(Ký, ghi rõ họ tên, đóng dấu)\n";

    out.close();
    return true;
}

// Hàm tạo giấy xác nhận ở định dạng DOCX (ví dụ đơn giản bằng cách ghi file văn bản)
bool generateCertificateDOCX(const CertificateData &data, const std::string &outputFile) {
    std::ofstream out(outputFile);
    if (!out.is_open()) {
        std::cerr << "Error: Cannot open output file for DOCX generation.\n";
        return false;
    }
    out << "TRƯỜNG ĐẠI HỌC " << data.schoolName << "\n";
    out << "PHÒNG ĐÀO TẠO\n";
    out << "📍 Địa chỉ: " << data.schoolAddress << "\n";
    out << "📞 Điện thoại: " << data.schoolPhone << " | 📧 Email: " << data.schoolEmail << "\n";
    out << "---------------------------------------------\n\n";
    out << "GIẤY XÁC NHẬN TÌNH TRẠNG SINH VIÊN\n\n";
    out << "Trường Đại học " << data.schoolName << " xác nhận:\n\n";
    out << "1. Thông tin sinh viên:\n";
    out << "- Họ và tên: " << data.studentName << "\n";
    out << "- Mã số sinh viên: " << data.studentID << "\n";
    out << "- Ngày sinh: " << data.studentDOB << "\n";
    out << "- Giới tính: " << data.studentGender << "\n";
    out << "- Khoa: " << data.studentFaculty << "\n";
    out << "- Chương trình đào tạo: " << data.studentProgram << "\n";
    out << "- Khóa: " << data.studentCourse << "\n\n";
    out << "2. Tình trạng sinh viên hiện tại:\n";
    out << "- Đang theo học\n";
    out << "- Đã hoàn thành chương trình, chờ xét tốt nghiệp\n";
    out << "- Đã tốt nghiệp\n";
    out << "- Bảo lưu\n";
    out << "- Đình chỉ học tập\n";
    out << "- Tình trạng khác\n\n";
    out << "3. Mục đích xác nhận:\n";
    out << "- Mục đích:" << data.confirmationPurpose << "\n\n";
    out << "4. Thời gian cấp giấy:\n";
    out << "- Giấy xác nhận có hiệu lực đến ngày: " << data.effectiveDate << "\n\n";
    out << "Xác nhận của Trường Đại học " << data.schoolName << "\n";
    out << "Ngày cấp: " << data.issueDate << "\n";
    out << "Trưởng Phòng Đào Tạo\n";
    out << "(Ký, ghi rõ họ tên, đóng dấu)\n";
    out.close();
    return true;
}

// Hàm tổng hợp để tạo giấy xác nhận theo định dạng đã chọn
bool generateCertificate(const CertificateData &data, const std::string &outputFile, CertificateFormat format) {
    if (format == CertificateFormat::MD) {
        return generateCertificateMarkdown(data, outputFile);
    } else if (format == CertificateFormat::DOCX) {
        return generateCertificateDOCX(data, outputFile);
    }
    return false;
}
